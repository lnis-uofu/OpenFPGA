#
# tclline: An attempt at a pure tcl readline.
#
# This base code taken from https://wiki.tcl-lang.org/20215 and
# https://wiki.tcl-lang.org/16139
#
# Author: HCG
# Licence: "as freely available as possible" https://wiki.tcl-lang.org/4381
#
# Modified by rjmcmahon: fixes history and multiple key sequences per input char (may not assume atomic)
# Also added ability to extend the completion handlers
#
# Note: The wiki.tcl.tk is broken with respect to creating a new page and reverting back to an old page
# Some comments from the previous wiki page no longer shown as the file upload seem to overwrite the entire wiki page.
#
# [LV] Actually, it isn't broken - but there might be a misunderstanding in terms of the functionality of file upload.
# The comments on each page are literally part of the page itself. The file upload replaces the complete contents of the page with new contents
# Thus, if the comments would be useful to keep, you should download the current wiki text page, replace the portion of the page containing
# code or whatever with the new contents, then upload the entire page. By just uploading tclline, you are effectively saying "the only thing
# I want on this page is the contents of this file".
#

# example usage in .tclshrc:
#
#   package require TclReadLine
#   ;# set ::TclReadLine::PROMPT {tclsh[info patchlevel] \[[pwd]\]% }  ;# no colour alternative
#   set ::TclReadLine::PROMPT {\033\[36mtclsh-[info patchlevel]\033\[0m \[\033\[34m[file tail [pwd]\033\[0m]\]\033\[31m % \033\[0m}
#   tailcall ::TclReadLine::interact


package provide TclReadLine 1.1

if {[catch {
    # Use Tclx if available:
    package require Tclx

    interp alias {} stty exec stty

    # Prevent sigint from killing our shell:
    signal ignore SIGINT
}]} {
    # else, fall back on Expect:
    package require Expect

    interp alias {} stty {} exp_stty

    # Prevent sigint from killing our shell:
    exp_trap SIG_IGN SIGINT
    # Handle terminal resize events:
    exp_trap ::TclReadLine::getColumns SIGWINCH
}


namespace eval TclReadLine {

    namespace export interact

    # Initialise our own env variables:
    variable PROMPT "OpenFPGA> "
    variable COMPLETION_MATCH ""
    
    # Support extensions to the completion handling
    # which will be called in list order.
    # Initialize with the "open sourced" TCL base handler
    # taken from the wiki page
    variable COMPLETION_HANDLERS [list TclReadLine::handleCompletionBase]

    #
    #  This value was determined by measuring 
    #  a cygwin over ssh. 
    #
    variable READLINE_LATENCY 10 ;# in ms
    
    variable CMDLINE ""
    variable CMDLINE_CURSOR 0
    variable CMDLINE_LINES 0
    variable CMDLINE_PARTIAL
    
    variable ALIASES
    array set ALIASES {}
    
    variable forever 0
    
    # Resource and history files:
    variable HISTORY_SIZE 100
    variable HISTORY_LEVEL 0
    variable HISTFILE $::env(HOME)/.tclline_history
    variable  RCFILE $::env(HOME)/.tcllinerc
}

proc TclReadLine::ESC {} {
    return "\033"
}

proc TclReadLine::shift {ls} {
    upvar 1 $ls LIST
    set ret [lindex $LIST 0]
    set LIST [lrange $LIST 1 end]
    return $ret
}

proc TclReadLine::readbuf {txt} {
    upvar 1 $txt STRING
    
    set ret [string index $STRING 0]
    set STRING [string range $STRING 1 end]
    return $ret
}

proc TclReadLine::goto {row {col 1}} {
    switch -- $row {
        "home" {set row 1}
    }
    print "[ESC]\[${row};${col}H" nowait
}

proc TclReadLine::gotocol {col} {
    print "\r" nowait
    if {$col > 0} {
        print "[ESC]\[${col}C" nowait
    }
}

proc TclReadLine::clear {} {
    print "[ESC]\[2J" nowait
    goto home
}

proc TclReadLine::clearline {} {
    print "[ESC]\[2K\r" nowait
}

proc TclReadLine::getColumns {} {
    variable COLUMNS
    set cols 0
    if {![catch {stty size} size]} {
        lassign $size rows cols
    } elseif {![catch {stty -a} err]} {
        # check for Linux stlye stty output
        if {[regexp {rows (= )?(\d+); columns (= )?(\d+)} $err - i1 rows i2 cols]} {
            return [set COLUMNS $cols]
        }
        # check for BSD style stty output
        if {[regexp { (\d+) rows; (\d+) columns;} $err - rows cols]} {
            return [set COLUMNS $cols]
        }
    }
    set COLUMNS $cols
}

proc TclReadLine::localInfo {args} {
    set v [uplevel _info $args]
    if { [string equal "script" [lindex $args 0]] } {
        if { [string equal $v $TclReadLine::ThisScript] } {
            return ""
        }
    }
    return $v
}

proc TclReadLine::localPuts {args} {
    
    set l [llength $args]
    if { 3 < $l } {
        return -code error "Error: wrong \# args"
    }
    
    if { 1 < $l } {
        if { [string equal "-nonewline" [lindex $args 0]] } {
            if { 2 < $l } {
                # we don't send to channel...
                eval _origPuts $args
            } else {
                set str [lindex $args 1]
                append TclReadLine::putsString $str ;# no newline...
            }
        } else {
            # must be a channel
            eval _origPuts $args
        }
    } else {
        append TclReadLine::putsString [lindex $args 0] "\n"
    }
}

proc TclReadLine::prompt {{txt ""}} {
    
    if { "" != [info var ::tcl_prompt1] } {
        rename ::puts ::_origPuts
        rename TclReadLine::localPuts ::puts
        variable putsString
        set putsString ""
        eval [set ::tcl_prompt1]
        set prompt $putsString
        rename ::puts TclReadLine::localPuts
        rename ::_origPuts ::puts
    } else {
        variable PROMPT
        set prompt [subst $PROMPT]
    }
    set txt "$prompt$txt"
    variable CMDLINE_LINES
    variable CMDLINE_CURSOR
    variable COLUMNS
    foreach {end mid} $CMDLINE_LINES break
    
    # Calculate how many extra lines we need to display.
    # Also calculate cursor position:
    set n -1
    set totalLen 0
    set visprompt [regsub -all {\x1b\[[0-9;]*[a-zA-Z]} $prompt {}]  ;# strip colour codes
    set cursorLen [expr {$CMDLINE_CURSOR+[string length $visprompt]}]
    set row 0
    set col 0
    
    # Render output line-by-line to $out then copy back to $txt:
    set found 0
    set out [list]
    foreach line [split $txt "\n"] {
        set len [expr {[string length $line]+1}]
        incr totalLen $len
        if {$found == 0 && $totalLen >= $cursorLen} {
            set cursorLen [expr {$cursorLen - ($totalLen - $len)}]
            set col [expr {$cursorLen % $COLUMNS}]
            set row [expr {$n + ($cursorLen / $COLUMNS) + 1}]
            
            if {$cursorLen >= $len} {
                set col 0
                incr row
            }
            set found 1
        }
        incr n [expr {int(ceil(double($len)/$COLUMNS))}]
        while {$len > 0} {
            lappend out [string range $line 0 [expr {$COLUMNS-1}]]
            set line [string range $line $COLUMNS end]
            set len [expr {$len-$COLUMNS}]
        }
    }
    set txt [join $out "\n"]
    set row [expr {$n-$row}]
    
    # Reserve spaces for display:
    if {$end} {
        if {$mid} {
            print "[ESC]\[${mid}B" nowait
        }
        for {set x 0} {$x < $end} {incr x} {
            clearline
            print "[ESC]\[1A" nowait
        }
    }
    clearline
    set CMDLINE_LINES $n
    
    # Output line(s):
    print "\r$txt"
    
    if {$row} {
        print "[ESC]\[${row}A" nowait
    }
    gotocol $col
    lappend CMDLINE_LINES $row
}

proc TclReadLine::print {txt {wait wait}} {
    # Sends output to stdout chunks at a time.
    # This is to prevent the terminal from
    # hanging if we output too much:
    while {[string length $txt]} {
        puts -nonewline [string range $txt 0 2047]
        set txt [string range $txt 2048 end]
        if {$wait == "wait"} {
            after 1
        }
    }
}

proc TclReadLine::unknown {args} {
    
    set name [lindex $args 0]
    set cmdline $TclReadLine::CMDLINE
    set cmd [string trim [regexp -inline {^\s*[^\s]+} $cmdline]]
    if {[info exists TclReadLine::ALIASES($cmd)]} {
        set cmd [regexp -inline {^\s*[^\s]+} $TclReadLine::ALIASES($cmd)]
    }
    
    set new [auto_execok $name]
    if {$new != ""} {
        set redir ""
        if {$name == $cmd && [info command $cmd] == ""} {
            set redir ">&@ stdout <@ stdin"
        }
        if {[catch {
            uplevel 1 exec $redir $new [lrange $args 1 end]} ret]
        } {
            return
        }
        return $ret
    }
    
    uplevel _unknown $args
}

proc TclReadLine::alias {word command} {
    variable ALIASES
    set ALIASES($word) $command
}

proc TclReadLine::unalias {word} {
    variable ALIASES
    array unset ALIASES $word
}

################################
# Key bindings
################################
proc TclReadLine::handleEscapes {} {
    
    variable CMDLINE
    variable CMDLINE_CURSOR
    
    upvar 1 keybuffer keybuffer
    set seq ""
    set found 0
    while {[set ch [readbuf keybuffer]] != ""} {
        append seq $ch

        switch -exact -- $seq {
            "\[A" { ;# Cursor Up (cuu1,up)
                handleHistory 1
                set found 1; break
            }
            "\[B" { ;# Cursor Down
                handleHistory -1
                set found 1; break
            }
            "\[C" { ;# Cursor Right (cuf1,nd)
                if {$CMDLINE_CURSOR < [string length $CMDLINE]} {
                    incr CMDLINE_CURSOR
                }
                set found 1; break
            }
            "\[D" { ;# Cursor Left
                if {$CMDLINE_CURSOR > 0} {
                    incr CMDLINE_CURSOR -1
                }
                set found 1; break
            }
            "\[H" -
            "\[7~" -
            "\[1~" { ;# home
                set CMDLINE_CURSOR 0
                set found 1; break
            }
            "\[3~" { ;# delete
                if {$CMDLINE_CURSOR < [string length $CMDLINE]} {
                    set CMDLINE [string replace $CMDLINE \
                                     $CMDLINE_CURSOR $CMDLINE_CURSOR]
                }
                set found 1; break
            }
            "\[F" -
            "\[K" -
            "\[8~" -
            "\[4~" { ;# end
                set CMDLINE_CURSOR [string length $CMDLINE]
                set found 1; break
            }
            "\[5~" { ;# Page Up
            }
            "\[6~" { ;# Page Down
            }
        }
    }
    return $found
}

proc TclReadLine::handleControls {} {
    
    variable CMDLINE
    variable CMDLINE_CURSOR
    
    upvar 1 char char
    upvar 1 keybuffer keybuffer
    
    # Control chars start at a == \u0001 and count up.
    switch -exact -- $char {
        \u0001 { ;# ^a
            set CMDLINE_CURSOR 0
        }
        \u0002 { ;# ^b
            if { $CMDLINE_CURSOR > 0 } {
                incr CMDLINE_CURSOR -1
            }
        }
        \u0004 { ;# ^d
            # should exit - if this is the EOF char, and the
            #   cursor is at the end-of-input
            if { 0 == [string length $CMDLINE] } {
                doExit
            }
            set CMDLINE [string replace $CMDLINE \
                             $CMDLINE_CURSOR $CMDLINE_CURSOR]
        }
        \u0005 { ;# ^e
            set CMDLINE_CURSOR [string length $CMDLINE]
        }
        \u0006 { ;# ^f
            if {$CMDLINE_CURSOR < [string length $CMDLINE]} {
                incr CMDLINE_CURSOR
            }
        }
        \u0007 { ;# ^g
            set CMDLINE ""
            set CMDLINE_CURSOR 0
        }
        \u000b { ;# ^k
            variable YANK
            set YANK  [string range $CMDLINE [expr {$CMDLINE_CURSOR  } ] end ]
            set CMDLINE [string range $CMDLINE 0 [expr {$CMDLINE_CURSOR - 1 } ]]
        }
        \u0019 { ;# ^y
            variable YANK
            if { [ info exists YANK ] } {
                set CMDLINE \
                    "[string range $CMDLINE 0 [expr {$CMDLINE_CURSOR - 1 }]]$YANK[string range $CMDLINE $CMDLINE_CURSOR end]"
            }
        }
        \u000e { ;# ^n
            handleHistory -1
        }
        \u0010 { ;# ^p
            handleHistory 1
        }
        \u0003 { ;# ^c
            # clear line
            set CMDLINE ""
            set CMDLINE_CURSOR 0
        }
        \u0008 -
        \u007f { ;# ^h && backspace ?
            if {$CMDLINE_CURSOR > 0} {
                incr CMDLINE_CURSOR -1
                set CMDLINE [string replace $CMDLINE \
                                 $CMDLINE_CURSOR $CMDLINE_CURSOR]
            }
        }
        \u001b { ;# ESC - handle escape sequences
            handleEscapes
        }
    }
    # Rate limiter:
    set keybuffer ""
}

proc TclReadLine::shortMatch {maybe} {
    # Find the shortest matching substring:
    set maybe [lsort $maybe]
    set shortest [lindex $maybe 0]
    foreach x $maybe {
        while {![string match $shortest* $x]} {
            set shortest [string range $shortest 0 end-1]
        }
    }
    return $shortest
}

proc TclReadLine::addCompletionHandler {completion_extension} {
    variable COMPLETION_HANDLERS
    set COMPLETION_HANDLERS [concat $completion_extension $COMPLETION_HANDLERS]
}

proc TclReadLine::delCompletionHandler {completion_extension} {
    variable COMPLETION_HANDLERS
    set COMPLETION_HANDLERS [lsearch -all -not -inline $COMPLETION_HANDLERS $completion_extension] 
}

proc TclReadLine::getCompletionHandler {} {
    variable COMPLETION_HANDLERS
    return "$COMPLETION_HANDLERS"
}

proc TclReadLine::handleCompletion {} {
    variable COMPLETION_HANDLERS
    foreach handler $COMPLETION_HANDLERS {
        if {[eval $handler] == 1} {
            break
        } 
    }
    return 
}

proc TclReadLine::handleCompletionBase {} {
    variable CMDLINE
    variable CMDLINE_CURSOR
    
    set vars ""
    set cmds ""
    set execs ""
    set files ""
    
    # First find out what kind of word we need to complete:
    set wordstart [string last " " $CMDLINE [expr {$CMDLINE_CURSOR-1}]]
    incr wordstart
    set wordend [string first " " $CMDLINE $wordstart]
    if {$wordend == -1} {
        set wordend end
    } else {
        incr wordend -1
    }
    set word [string range $CMDLINE $wordstart $wordend]
    
    if {[string trim $word] == ""} return
    
    set firstchar [string index $word 0]
    
    # Check if word is a variable:
    if {$firstchar == "\$"} {
        set word [string range $word 1 end]
        incr wordstart
        
        # Check if it is an array key:proc
        
        set x [string first "(" $word]
        if {$x != -1} {
            set v [string range $word 0 [expr {$x-1}]]
            incr x
            set word [string range $word $x end]
            incr wordstart $x
            if {[uplevel \#0 "array exists $v"]} {
                set vars [uplevel \#0 "array names $v $word*"]
            }
        } else {
            foreach x [uplevel \#0 {info vars}] {
                if {[string match $word* $x]} {
                    lappend vars $x
                }
            }
        }
    } else {
        # Check if word is possibly a path:
        if {$firstchar == "/" || $firstchar == "." || $wordstart != 0} {
            set files [glob -nocomplain -- $word*]
        }
        if {$files == ""} {
            # Not a path then get all possibilities:
            if {$firstchar == "\[" || $wordstart == 0} {
                if {$firstchar == "\["} {
                    set word [string range $word 1 end]
                    incr wordstart
                }
                # Check executables:
                foreach dir [split $::env(PATH) :] {
                    foreach f [glob -nocomplain -directory $dir -- $word*] {
                        set exe [string trimleft [string range $f \
                                                      [string length $dir] end] "/"]
                        
                        if {[lsearch -exact $execs $exe] == -1} {
                            lappend execs $exe
                        }
                    }
                }
                # Check commands:
                foreach x [info commands] {
                    if {[string match $word* $x]} {
                        lappend cmds $x
                    }
                }
            } else {
                # Check commands anyway:
                foreach x [info commands] {
                    if {[string match $word* $x]} {
                        lappend cmds $x
                    }
                }
            }
        }
        if {$wordstart != 0} {
            # Check variables anyway:
            set x [string first "(" $word]
            if {$x != -1} {
                set v [string range $word 0 [expr {$x-1}]]
                incr x
                set word [string range $word $x end]
                incr wordstart $x
                if {[uplevel \#0 "array exists $v"]} {
                    set vars [uplevel \#0 "array names $v $word*"]
                }
            } else {
                foreach x [uplevel \#0 {info vars}] {
                    if {[string match $word* $x]} {
                        lappend vars $x
                    }
                }
            }
        }
    }
    
    variable COMPLETION_MATCH
    set maybe [concat $vars $cmds $execs $files]
    set shortest [shortMatch $maybe]
    if {"$word" == "$shortest"} {
        if {[llength $maybe] > 1 && $COMPLETION_MATCH != $maybe} {
            set COMPLETION_MATCH $maybe
            clearline
            set temp ""
            foreach {match format} {
                vars  "35"
                cmds  "1;32"
                execs "32"
                files "0"
            } {
                if {[llength [set $match]]} {
                    append temp "[ESC]\[${format}m"
                    foreach x [set $match] {
                        append temp "[file tail $x] "
                    }
                    append temp "[ESC]\[0m"
                }
            }
            print "\n$temp\n"
        }
    } else {
        if {[file isdirectory $shortest] &&
            [string index $shortest end] != "/"} {
            append shortest "/"
        }
        if {$shortest != ""} {
            set CMDLINE \
                [string replace $CMDLINE $wordstart $wordend $shortest]
            set CMDLINE_CURSOR \
                [expr {$wordstart+[string length $shortest]}]
        } elseif { $COMPLETION_MATCH != " not found "} {
            set COMPLETION_MATCH " not found "
            print "\nNo match found.\n"
        }
    }
}

proc TclReadLine::handleHistory {x} {
    variable HISTORY_LEVEL
    variable HISTORY_SIZE
    variable CMDLINE
    variable CMDLINE_CURSOR
    variable CMDLINE_PARTIAL
    
    set maxid [expr {[history nextid] - 1}]
    if {$maxid > 0} {
        #
        #  Check for a top level command line and history event
        #  Store this command line locally (i.e. don't use the history stack)
        #
        if {$HISTORY_LEVEL == 0} {
            set CMDLINE_PARTIAL $CMDLINE
        } 
        incr HISTORY_LEVEL $x
        #
        #  Note:  HISTORY_LEVEL is used to offset into
        #  the history events.  It will be reset to zero 
        #  when a command is executed by tclline.
        #  
        #  Check the three bounds of
        #  1) HISTORY_LEVEL <= 0 - Restore the top level cmd line (not in history stack)
        #  2) HISTORY_LEVEL > HISTORY_SIZE
        #  3) HISTORY_LEVEL > maxid
        #
        if {$HISTORY_LEVEL <= 0} {
            set HISTORY_LEVEL 0 
            if {[info exists CMDLINE_PARTIAL]} {
                set CMDLINE $CMDLINE_PARTIAL
                set CMDLINE_CURSOR [string length $CMDLINE]
            }
            return
        } elseif {$HISTORY_LEVEL > $maxid} {
            set HISTORY_LEVEL $maxid
        } elseif {$HISTORY_LEVEL > $HISTORY_SIZE} {
            set HISTORY_LEVEL $HISTORY_SIZE
        } 
        set id [expr {($maxid + 1) - $HISTORY_LEVEL}]
        set cmd [expr {$id > $maxid ? "" : [history event $id]}]
        set CMDLINE $cmd
        set CMDLINE_CURSOR [string length $cmd]
    }
}

################################
# History handling functions
################################

proc TclReadLine::getHistory {} {
    variable HISTORY_SIZE
    
    set l [list]
    set e [history nextid]
    set i [expr {$e - $HISTORY_SIZE}]
    if {$i <= 0} {
        set i 1
    }
    for { set i } {$i < $e} {incr i} {
        lappend l [history event $i]
    }
    return $l
}

proc TclReadLine::setHistory {hlist} {
    foreach event $hlist {
        history add $event
    }
}

################################
# main()
################################

proc TclReadLine::rawInput {} {
    fconfigure stdin -buffering none -blocking 0
    fconfigure stdout -buffering none -translation crlf
    stty raw -echo
}

proc TclReadLine::lineInput {} {
    fconfigure stdin -buffering line -blocking 1
    fconfigure stdout -buffering line
    stty -raw echo
}

proc TclReadLine::doExit {{code 0}} {
    variable HISTFILE
    variable HISTORY_SIZE 

    # Reset terminal:
    #print "[ESC]c[ESC]\[2J" nowait
    
    restore ;# restore "info' command -
    lineInput
    
    set hlist [getHistory]
    #
    # Get rid of the TclReadLine::doExit, shouldn't be more than one
    #
    set hlist [lsearch -all -not -inline $hlist "TclReadLine::doExit"]
    set hlistlen [llength $hlist]
    if {$hlistlen > 0} {
        set f [open $HISTFILE w]
        if {$hlistlen > $HISTORY_SIZE} {
            set hlist [lrange $hlist [expr {($hlistlen - $HISTORY_SIZE - 1)}] end]
        }
        foreach x $hlist {
            # Escape newlines:
            puts $f [string map {
                \n "\\n"
                "\\" "\\b"
            } $x]
        }
        close $f
    }
    
    exit $code
}

proc TclReadLine::restore {} {
    lineInput
    rename ::unknown TclReadLine::unknown
    rename ::_unknown ::unknown
}

proc TclReadLine::interact {} {

    rename ::unknown ::_unknown
    rename TclReadLine::unknown ::unknown
    
    variable RCFILE
    if {[file exists $RCFILE]} {
        source $RCFILE
    }

    # Load history if available:
    # variable HISTORY
    variable HISTFILE
    variable HISTORY_SIZE
    history keep $HISTORY_SIZE
    
    if {[file exists $HISTFILE]} {
        set f [open $HISTFILE r]
        set hlist [list]
        foreach x [split [read $f] "\n"] {
            if {$x != ""} {
                # Undo newline escapes:
                lappend hlist [string map {
                    "\\n" \n
                    "\\\\" "\\"
                    "\\b" "\\"
                } $x]
            }
        }
        setHistory $hlist
        unset hlist
        close $f
    }
    
    rawInput
    
    # This is to restore the environment on exit:
    # Do not unalias this!
    alias exit TclReadLine::doExit
    
    variable ThisScript [info script]
    
    tclline ;# emit the first prompt
    
    fileevent stdin readable TclReadLine::tclline
    variable forever
    vwait TclReadLine::forever
    
    restore
}


proc TclReadLine::check_partial_keyseq {buffer} {
    variable READLINE_LATENCY
    upvar $buffer keybuffer

    #
    # check for a partial esc sequence as tclline expects the whole sequence
    #
    if {[string index $keybuffer 0] == [ESC]} {
        #
        # Give extra time to read partial key sequences
        # 
        set timer  [expr {[clock clicks -milliseconds] + $READLINE_LATENCY}]
        while {[clock clicks -milliseconds] < $timer } {
            append keybuffer [read stdin]
        }
    }
}

proc TclReadLine::tclline {} {
    variable COLUMNS
    variable CMDLINE_CURSOR
    variable CMDLINE
    
    set char ""
    set keybuffer [read stdin]
    set COLUMNS [getColumns]
    
    check_partial_keyseq keybuffer
    
    while {$keybuffer != ""} {
        if {[eof stdin]} return
        set char [readbuf keybuffer]
        if {$char == ""} {
            # Sleep for a bit to reduce CPU overhead:
            after 40
            continue
        }
        
        if {[string is print $char]} {
            set x $CMDLINE_CURSOR
            
            if {$x < 1 && [string trim $char] == ""} continue
            
            set trailing [string range $CMDLINE $x end]
            set CMDLINE [string replace $CMDLINE $x end]
            append CMDLINE $char
            append CMDLINE $trailing
            incr CMDLINE_CURSOR
        } elseif {$char == "\t"} {
            handleCompletion
        } elseif {$char == "\n" || $char == "\r"} {
            if {[info complete $CMDLINE] &&
                [string index $CMDLINE end] != "\\"} {
                lineInput
                print "\n" nowait
                uplevel \#0 {
                    
                    # Handle aliases:
                    set cmdline $TclReadLine::CMDLINE
                    #
                    # Add the cmd line to history before doing any substitutions
                    # 
                    history add $cmdline
                    set cmd [string trim [regexp -inline {^\s*[^\s]+} $cmdline]]
                    if {[info exists TclReadLine::ALIASES($cmd)]} {
                        regsub -- "(?q)$cmd" $cmdline $TclReadLine::ALIASES($cmd) cmdline
                    }
                    
                    # Perform glob substitutions:
                    set cmdline [string map {
                        "\\*" \0
                        "\\~" \1
                    } $cmdline]
                    #
                    # Prevent glob substitution of *,~ for tcl commands
                    #
                    if {[info commands $cmd] != ""} {
                        set cmdline [string map {
                            "\*" \0
                            "\~" \1
                        } $cmdline]
                    }
                    while {[regexp -indices \
                                {([\w/\.]*(?:~|\*)[\w/\.]*)+} $cmdline x]
                       } {
                        foreach {i n} $x break
                        set s [string range $cmdline $i $n]
                        set x [glob -nocomplain -- $s]
                        
                        # If glob can't find anything then don't do
                        # glob substitution, pass * or ~ as literals:
                        if {$x == ""} {
                            set x [string map {
                                "*" \0
                                "~" \1
                            } $s]
                        }
                        set cmdline [string replace $cmdline $i $n $x]
                    }
                    set cmdline [string map {
                        \0 "*"
                        \1 "~"
                    } $cmdline]
                    
                    rename ::info ::_info
                    rename TclReadLine::localInfo ::info

                    # Reset HISTORY_LEVEL before next command
                    set TclReadLine::HISTORY_LEVEL 0
                    if {[info exists TclReadLine::CMDLINE_PARTIAL]} {
                        unset TclReadLine::CMDLINE_PARTIAL
                    }
                    
                    # Run the command:
                    set code [catch $cmdline res]
                    rename ::info TclReadLine::localInfo
                    rename ::_info ::info
                    if {$code == 1} {
                        TclReadLine::print "$::errorInfo\n"
                    } else {
                        TclReadLine::print "$res\n"
                    }
                    
                    set TclReadLine::CMDLINE ""
                    set TclReadLine::CMDLINE_CURSOR 0
                    set TclReadLine::CMDLINE_LINES {0 0}
                } ;# end uplevel
                rawInput
            } else {
                set x $CMDLINE_CURSOR
                
                if {$x < 1 && [string trim $char] == ""} continue
                
                set trailing [string range $CMDLINE $x end]
                set CMDLINE [string replace $CMDLINE $x end]
                append CMDLINE $char
                append CMDLINE $trailing
                incr CMDLINE_CURSOR
            }
        } else {
            handleControls
        }
    }
    prompt $CMDLINE
}

# start immediately if invoked as a script:
if {!$::tcl_interactive && [info script] eq $::argv0} {
    TclReadLine::interact
}

# Put the following in your .tclshrc
#  if {$::tcl_interactive} {
#    package require TclReadLine
#    TclReadLine::interact
#  }



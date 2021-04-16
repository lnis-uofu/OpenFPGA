library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- c2003 Franks Development, LLC
-- http://www.franks-development.com
-- !This source is distributed under the terms & conditions specified at opencores.org
-- 
-- Please see the file "StepperMotorWiring.bmp" for info on connecting 4 & 6 
-- wire motors to your device.  This source should drive either type, though connection
-- to 4-wire motors requires significantly more FET's to buffer outputs.  The
-- circuitry for 6-wire motors is more straightforward.  The colors specified are 
-- standard to many brands of stepper motors.  If you get 1-2-3-4 on the FET's connected
-- to 4-3-2-1 on your logic device (backwards), the motor will simply rotate backwards.
-- if out of order, the motor won't rotate at all.  Motors come in many dirrerent ratings
-- of degrees rotation per step; some offer exceptional resolution very inexpensively.
-- 
-- It is important to note that most logic operates at multiple megahertz.  Given
-- a steper motor at 100 steps per revolution, and a clock of 10MHz, running the 
-- motor at the full clock rate would equal 100,000 rps or 6 million revolutions per minute
-- obviously, motors don't do this.  Most steppers are designed for fine resolution at low
-- speeds.  Thus we employ a really big clock divider to get things operating at speeds
-- of which the motor is capable.  Check your motor ratings for a usefull value.
-- in lieu of ratings, 100 rpm is usually achieveable, so plan accordingly.
--
-- Another practical consideration is the threshold voltage of the FET's used to
-- buffer the logic outputs & provide current drive to the motor.  Most power
-- FET's have a threshold voltage in the 5-12V range (or even higher),
-- while logic devices now run in the 3.3V and lower range.  Thus, you must be 
-- careful to choose a FET with a low threshold voltage, or a level-converter must
-- be utilized between the logic output and the gate of the drive FET's.
-- Practical tip: FET's with very high currect handling capabilities, in general
-- (so be sure to read the datasheet before you buy), tend to handle larger currents
-- for any given gate voltage.  This means that in many cases, even if Vgs is rated at 5 volts
-- if your stepper uses relatively low current, the FET's may still drive it at 3.3V or lower.
-- In the worst case, you are likely going to need a high voltage power supply to drive the
-- motors anyway, so you can "double-up" low power FET's to drive the gates of the power FET's
-- with a higher voltage.  In effect, the low-power FET's are wired as inverters with a low
-- swithcing voltage.  Contact Franks Development for a napkin-sketch if you aren't familiar
-- with how to do that.  Good luck.
--
-- One of the most advantageous abilities of stepper motors is the ability to 
-- provide static holding force in any position.  Of course this consumes power
-- and heats the motor up significantly (though steppers are rated to handle this)
-- use of the "static holding" input port will specify this behavior.  Be aware,
-- however, that the motor will dissipate power if left energized for long periods
-- without (or without for that matter) rotating.  This will make them hot!  They are 
-- usually designed for it, but it is a consideration, especially if in a small,
-- sealed, enclosure, excess heat may make your logic cease functioning correctly at 
-- some point.

entity StepperMotorPorts is
    Port (	
	 			StepDrive : out std_logic_vector(3 downto 0); -- the 4 output to drive the MOSFETS driving the coils in the motor.
		 		clock : in std_logic;								 -- clock input to logic device
		 		Direction : in std_logic;							 -- spin clockwise or counter-clockwise? (actual direction depends on correct hookup/pin assignements)
		 		StepEnable : in std_logic;							 -- move a single step on next clock divider rollover.  leave high for a single clock to get a single step.  If high across rollover, may get two steps
		 		ProvideStaticHolding : in std_logic			    -- leave motor coils energized when not rotating, so that counter-torque is applied if attempt to move shaft
		);
end StepperMotorPorts;

architecture StepDrive of StepperMotorPorts is

	signal state : std_logic_vector(1 downto 0);				-- simple state machine, 4 states
	signal StepCounter : std_logic_vector(31 downto 0);   -- most motors won't spin extrordinarially fast, so this slows the clock input way down
	constant StepLockOut : std_logic_vector(31 downto 0) := "00000000000000110000110101000000"; --rollover for the counter, to get a non-binary delay time divider
	signal InternalStepEnable : std_logic;						-- used to capture a step enable even when we are in the wait loop for the clock divider.

begin

	process(clock)
	begin

		if ( (clock'Event) and (clock = '1') ) then		 --on clock

			StepCounter <= StepCounter + "0000000000000000000000000000001"; --move the delay counter

			if (StepEnable = '1') then 

				InternalStepEnable <= '1'; 	-- capture any requests for a step, even while we are waiting...

			end if;

			if (StepCounter >= StepLockOut) then

				StepCounter <= "00000000000000000000000000000000";		-- if we just roll-ed over, then it's time to do something

				if (ProvideStaticHolding = '1') then --should we leave coils in energized state by defaul or not?

					StepDrive <= "0000";

				else

					StepDrive <= "1111";

				end if;
				
				if (InternalStepEnable = '1') then -- are we supposed to step on this clock?

					InternalStepEnable <= StepEnable; 	-- InternalStepEnable togles at the speed of the clock divider rollover, trailing the 
																	-- external StepEnable by less than or equal to one rollover.
																	-- Putting this inside the "if internal=1" makes us wait until after move to turn off,
																	-- so we move at least once for each pulse of external step enable line.
					
					if (Direction = '1') then state <= state + "01"; end if; -- to change the direction of a stepper motor, you energize 
					if (Direction = '0') then state <= state - "01"; end if; -- the coils in the opposite pattern, so just run states backwards
																								-- this also allows a change of direction at any arbitrary point
					case state is 

						when "00" =>

							StepDrive <= "1010";			-- these states follow proper pattern of coil energizing for turning steppers
							
						when "01" =>
		
							StepDrive <= "1001";
							
						when "10" =>
		
							StepDrive <= "0101";			
							
						when "11" =>
		
							StepDrive <= "0110";
							
						when others =>

					end case; --state
	
				end if;
	
			end if;

		end if;

	end process;

end StepDrive;

# Top level keywords defining the begin of a cell definition.
top_level = [
    "model",
    "inputs",
    "outputs",
    "names",
    "latch",
    "subckt",
]

# Keywords defining cell attributes / parameters. Those can be specified for
# each cell multiple times. Parameter names and values are stored in a dict
# under the parsed blif data.
#
# For example: the construct ".param MODE SYNC" will add to the dict under
# the key "param" entry "MODE":"SYNC".
#
sub_level = [
    "attr",
    "param",
]


def parse_blif(f):
    current = None

    data = {}

    def add(d):
        if d['type'] not in data:
            data[d['type']] = []
        data[d['type']].append(d)

    current = None
    for oline in f:
        line = oline
        if '#' in line:
            line = line[:line.find('#')]
        line = line.strip()
        if not line:
            continue

        if line.startswith("."):
            args = line.split(" ", maxsplit=1)
            if len(args) < 2:
                args.append("")

            ctype = args.pop(0)
            assert ctype.startswith("."), ctype
            ctype = ctype[1:]

            if ctype in top_level:
                if current:
                    add(current)
                current = {
                    'type': ctype,
                    'args': args[-1].split(),
                    'data': [],
                }
            elif ctype in sub_level:
                if ctype not in current:
                    current[ctype] = {}
                key, value = args[-1].split(maxsplit=1)
                current[ctype][key] = value
            else:
                current[ctype] = args[-1].split()
            continue
        current['data'].append(line.strip().split())

    if current:
        add(current)

    assert len(data['inputs']) == 1
    data['inputs'] = data['inputs'][0]
    assert len(data['outputs']) == 1
    data['outputs'] = data['outputs'][0]
    return data

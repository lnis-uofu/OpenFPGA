@0xab66996576fe2ea9;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("QLMem_db");
struct FabricBitstreamQLMem {
    # holds caches data for a FabricBitstream object with flattened ql_memory_bank config
    numRegions @0 :UInt8 = 1;
    invalidRegionIds @1 :List(UInt8);
    regionBitIds @2 :List(UInt64);
    numBits @3 :UInt64;
    invalidBitIds @4 :List(UInt64);
    configBitIds @5 :List(UInt64);
    useAddress @6 :Bool = true;
    useWlAddress @7 :Bool = true;
    addressLength @8 :UInt8 = 64;
    wlAddressLength @9 :UInt8 = 64;
    bitAddress1bits @10 :List(List(UInt64));
    bitAddressXbits @11 :List(List(UInt64));
    bitWlAddress1bits @12 :List(List(UInt64));
    bitWlAddressXbits @13 :List(List(UInt64));
    bitDins @14 :List(Text);
}
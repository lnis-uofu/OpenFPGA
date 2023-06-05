@0xab66996576fe2e19;
using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("QLMem_db");
struct FabricBitstreamQLMem {
    # holds caches data for a FabricBitstream object with flattened ql_memory_bank config
    configBitIds @0 :List(UInt64);
    bitAddress1bits @1 :List(List(UInt64));
    bitAddressXbits @2 :List(List(UInt64));
    bitWlAddress1bits @3 :List(List(UInt64));
}
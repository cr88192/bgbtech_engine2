/*
Copyright (C) 2015-2016 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

struct {
char *frmt;
char *name;
}bs2c_insns[]={
{"00",        "ADDI"},
{"01",        "SUBI"},
{"02",        "MULI"},
{"03",        "ANDI"},
{"04",        "ORI"},
{"05",        "XORI"},
{"06",        "SHLI"},
{"07",        "SARI"},
{"08",        "ADDL"},
{"09",        "SUBL"},
{"0A",        "MULL"},
{"0B",        "ANDL"},
{"0C",        "ORL"},
{"0D",        "XORL"},
{"0E",        "SHLL"},
{"0F",        "SARL"},
{"10",        "ADDF"},
{"11",        "SUBF"},
{"12",        "MULF"},
{"13",        "DIVF"},
{"14",        "ADDD"},
{"15",        "SUBD"},
{"16",        "MULD"},
{"17",        "DIVD"},
{"18",        "NEGI"},
{"19",        "NEGL"},
{"1A",        "NEGF"},
{"1B",        "NEGD"},
{"1C",        "NOTI"},
{"1D",        "NOTL"},
{"1E",        "LNTI"},
{"1F",        "LNTL"},
{"20Ix",      "LDI"},
{"21Ix",      "LDL"},
{"22Ix",      "LDF"},
{"23Ix",      "LDD"},
{"24Ix",      "STI"},
{"25Ix",      "STL"},
{"26Ix",      "STF"},
{"27Ix",      "STD"},
{"28Ix",      "LDA"},
{"29Ix",      "STA"},
{"2AZx",      "LDC"},
{"2BJx",      "MVA"},
{"2CJx",      "MVI"},
{"2DJx",      "MVL"},
{"2EJx",      "MVF"},
{"2FJx",      "MVD"},
{"30Ax",      "JEQ"},
{"31Ax",      "JNE"},
{"32Ax",      "JLT"},
{"33Ax",      "JGT"},
{"34Ax",      "JLE"},
{"35Ax",      "JGE"},

// {"36ZoAx",    "JCMP"},

{"3640",        "LDIXI.B"},
{"3641",        "LDIXL.B"},
{"3642",        "LDIXF.B"},
{"3643",        "LDIXD.B"},
{"3644",        "STIXI.B"},
{"3645",        "STIXL.B"},
{"3646",        "STIXF.B"},
{"3647",        "STIXD.B"},
{"3648",        "LDIXSB.B"},
{"3649",        "LDIXUB.B"},
{"364A",        "LDIXSS.B"},
{"364B",        "LDIXUS.B"},
{"364C",        "LDIXA.B"},
{"364D",        "STIXA.B"},
{"364E",        "STIXB.B"},
{"364F",        "STIXS.B"},
{"3650Ci",      "LDIXIC.B"},
{"3651Ci",      "LDIXLC.B"},
{"3652Ci",      "LDIXFC.B"},
{"3653Ci",      "LDIXDC.B"},
{"3654Ci",      "STIXIC.B"},
{"3655Ci",      "STIXLC.B"},
{"3656Ci",      "STIXFC.B"},
{"3657Ci",      "STIXDC.B"},
{"3658Ci",      "LDIXSBC.B"},
{"3659Ci",      "LDIXUBC.B"},
{"365ACi",      "LDIXSSC.B"},
{"365BCi",      "LDIXUSC.B"},
{"365CCi",      "LDIXAC.B"},
{"365DCi",      "STIXAC.B"},
{"365ECi",      "STIXBC.B"},
{"365FCi",      "STIXSC.B"},

{"8C3C",        "CMPA.P"},

{"8C40",        "LDIXI.P"},
{"8C41",        "LDIXL.P"},
{"8C42",        "LDIXF.P"},
{"8C43",        "LDIXD.P"},
{"8C44",        "STIXI.P"},
{"8C45",        "STIXL.P"},
{"8C46",        "STIXF.P"},
{"8C47",        "STIXD.P"},
{"8C48",        "LDIXSB.P"},
{"8C49",        "LDIXUB.P"},
{"8C4A",        "LDIXSS.P"},
{"8C4B",        "LDIXUS.P"},
{"8C4C",        "LDIXA.P"},
{"8C4D",        "STIXA.P"},
{"8C4E",        "STIXB.P"},
{"8C4F",        "STIXS.P"},
{"8C50Ci",      "LDIXIC.P"},
{"8C51Ci",      "LDIXLC.P"},
{"8C52Ci",      "LDIXFC.P"},
{"8C53Ci",      "LDIXDC.P"},
{"8C54Ci",      "STIXIC.P"},
{"8C55Ci",      "STIXLC.P"},
{"8C56Ci",      "STIXFC.P"},
{"8C57Ci",      "STIXDC.P"},
{"8C58Ci",      "LDIXSBC.P"},
{"8C59Ci",      "LDIXUBC.P"},
{"8C5ACi",      "LDIXSSC.P"},
{"8C5BCi",      "LDIXUSC.P"},
{"8C5CCi",      "LDIXAC.P"},
{"8C5DCi",      "STIXAC.P"},
{"8C5ECi",      "STIXBC.P"},
{"8C5FCi",      "STIXSC.P"},

{"8CE130",      "LEARI.P"},
{"8CE131",      "LEARL.P"},
{"8CE132",      "LEARF.P"},
{"8CE133",      "LEARD.P"},
{"8CE134",      "LEARA.P"},
{"8CE135",      "LEARB.P"},
{"8CE136",      "LEARS.P"},
{"8CE138Ci",    "LEARIC.P"},
{"8CE139Ci",    "LEARLC.P"},
{"8CE13ACi",    "LEARFC.P"},
{"8CE13BCi",    "LEARDC.P"},
{"8CE13CCi",    "LEARAC.P"},
{"8CE13DCi",    "LEARBC.P"},
{"8CE13ECi",    "LEARSC.P"},

{"8CE150",      "LDDRI.P"},
{"8CE151",      "LDDRL.P"},
{"8CE152",      "LDDRF.P"},
{"8CE153",      "LDDRD.P"},
{"8CE154",      "STDRI.P"},
{"8CE155",      "STDRL.P"},
{"8CE156",      "STDRF.P"},
{"8CE157",      "STDRD.P"},
{"8CE158",      "LDDRSB.P"},
{"8CE159",      "LDDRUB.P"},
{"8CE15A",      "LDDRSS.P"},
{"8CE15B",      "LDDRUS.P"},
{"8CE15C",      "LDDRA.P"},
{"8CE15D",      "STDRA.P"},
{"8CE15E",      "STDRB.P"},
{"8CE15F",      "STDRS.P"},

{"AA7CGx",      "LDOS.TN"},
{"AA7DGx",      "STOS.TN"},
{"AA50Ci",      "LDIXIC.TN"},
{"AA51Ci",      "LDIXLC.TN"},
{"AA52Ci",      "LDIXFC.TN"},
{"AA53Ci",      "LDIXDC.TN"},
{"AA54Ci",      "STIXIC.TN"},
{"AA55Ci",      "STIXLC.TN"},
{"AA56Ci",      "STIXFC.TN"},
{"AA57Ci",      "STIXDC.TN"},
{"AA58Ci",      "LDIXSBC.TN"},
{"AA59Ci",      "LDIXUBC.TN"},
{"AA5ACi",      "LDIXSSC.TN"},
{"AA5BCi",      "LDIXUSC.TN"},
{"AA5CCi",      "LDIXAC.TN"},
{"AA5DCi",      "STIXAC.TN"},
{"AA5ECi",      "STIXBC.TN"},
{"AA5FCi",      "STIXSC.TN"},

{"36",          "HBC"},

{"37Ax",      "JMP"},
{"38",        "CMPI"},
{"39",        "CMPL"},
{"3A",        "CMPF"},
{"3B",        "CMPD"},
{"3C",        "CMPA"},
{"3D",        "CMP2A"},
{"3E",        "CMP2F"},
{"3F",        "CMP2D"},
{"40",        "LDIXI"},
{"41",        "LDIXL"},
{"42",        "LDIXF"},
{"43",        "LDIXD"},
{"44",        "STIXI"},
{"45",        "STIXL"},
{"46",        "STIXF"},
{"47",        "STIXD"},
{"48",        "LDIXSB"},
{"49",        "LDIXUB"},
{"4A",        "LDIXSS"},
{"4B",        "LDIXUS"},
{"4C",        "LDIXA"},
{"4D",        "STIXA"},
{"4E",        "STIXB"},
{"4F",        "STIXS"},
{"50Ci",      "LDIXIC"},
{"51Ci",      "LDIXLC"},
{"52Ci",      "LDIXFC"},
{"53Ci",      "LDIXDC"},
{"54Ci",      "STIXIC"},
{"55Ci",      "STIXLC"},
{"56Ci",      "STIXFC"},
{"57Ci",      "STIXDC"},
{"58Ci",      "LDIXSBC"},
{"59Ci",      "LDIXUBC"},
{"5ACi",      "LDIXSSC"},
{"5BCi",      "LDIXUSC"},
{"5CCi",      "LDIXAC"},
{"5DCi",      "STIXAC"},
{"5ECi",      "STIXBC"},
{"5FCi",      "STIXSC"},

{"61A0",        "NX.POPI"},
{"61A1",        "NX.POPL"},
{"61A2",        "NX.POPF"},
{"61A3",        "NX.POPD"},
{"61A4",        "NX.DUPI"},
{"61A5",        "NX.DUPL"},
{"61A6",        "NX.DUPF"},
{"61A7",        "NX.DUPD"},
{"61A8",        "NX.POPA"},
{"61A9",        "NX.DUPA"},
// {"61AA",        "NX.SWAPA"},
{"61AB",        "NX.PUSHA"},
{"61AC",        "NX.PUSHI"},
{"61AD",        "NX.PUSHL"},
{"61AE",        "NX.PUSHF"},
{"61AF",        "NX.PUSHD"},

{"60",        "HNOEX"},
{"61",        "HNOEX1"},
{"62",        "HNOEX2"},
{"63",        "HNOEX3"},
{"64Zx",      "RETC"},
{"66",        "UCMPI"},
{"67",        "UCMPL"},
{"68Ci",      "CMPIC"},
{"69Ix",      "CMPIL"},
{"6AKx",      "CMPILC"},
{"6BJx",      "CMPILL"},
{"6C",        "INCI"},
{"6D",        "DECI"},
{"6EIx",      "INCIL"},
{"6FIx",      "DECIL"},

{"70Gx",      "CALLG"},
{"71Gx",      "CALLA"},
{"72Gx",      "CALLV"},
{"73Gj",      "CALLL"},
{"74",        "RETI"},
{"75",        "RETL"},
{"76",        "RETF"},
{"77",        "RETD"},
{"78",        "RETA"},
{"79",        "RETV"},
{"7AZi",      "RET2"},
{"7B",        "LABEL"},
{"7CGx",      "LDOS"},
{"7DGx",      "STOS"},
{"7EGx",      "LDGS"},
{"7FGx",      "STGS"},
{"80Zi",      "LDRL"},
{"81Gx",      "LDROS"},
{"82Gx",      "LDRGS"},
{"83Zi",      "LDRLX"},
{"84Zi",      "LDLX"},
{"85Zi",      "STLX"},
{"86Zi",      "LDDRLX"},
{"87Zi",      "STDRLX"},
//{"86Zn",      "DCLX"},
//{"87Zn",      "DELX"},
{"88Gx",      "NEWOBJ"},
{"89Zn",      "NEWARR"},
{"8A",        "DELOBJ"},
{"8B",        "DELARR"},

{"8C",        "HPTR"},

{"8EGj",      "LDOSL"},
{"8FGj",      "STOSL"},

// {"8C",        "MOVOBJ"},
// {"8D",        "MOVARR"},
// {"8E",        "INCREF"},
// {"8F",        "DECREF"},
{"90",        "CVTI2L"},
{"91",        "CVTI2F"},
{"92",        "CVTI2D"},
{"93",        "CVTL2I"},
{"94",        "CVTL2F"},
{"95",        "CVTL2D"},
{"96",        "CVTF2I"},
{"97",        "CVTF2L"},
{"98",        "CVTF2D"},
{"99",        "CVTD2I"},
{"9A",        "CVTD2L"},
{"9B",        "CVTD2F"},
{"9C",        "CVTSB2I"},
{"9D",        "CVTUB2I"},
{"9E",        "CVTSS2I"},
{"9F",        "CVTUS2I"},
{"A0",        "POPI"},
{"A1",        "POPL"},
{"A2",        "POPF"},
{"A3",        "POPD"},
{"A4",        "DUPI"},
{"A5",        "DUPL"},
{"A6",        "DUPF"},
{"A7",        "DUPD"},
{"A8",        "POPA"},
{"A9",        "DUPA"},
// {"AA",        "SWAPA"},
{"AA",        "HTNULL"},
{"AB",        "PUSHA"},
{"AC",        "PUSHI"},
{"AD",        "PUSHL"},
{"AE",        "PUSHF"},
{"AF",        "PUSHD"},
{"B0Cj",      "ADDIC"},
{"B1Cj",      "SUBIC"},
{"B2Ci",      "MULIC"},
{"B3Ci",      "ANDIC"},
{"B4Ci",      "ORIC"},
{"B5Ci",      "XORIC"},
{"B6Ci",      "SHLIC"},
{"B7Ci",      "SARIC"},
{"B8Ix",      "ADDIL"},
{"B9Ix",      "SUBIL"},
{"BAIx",      "MULIL"},
{"BBIx",      "ANDIL"},
{"BCIx",      "ORIL"},
{"BDIx",      "XORIL"},
{"BEIx",      "SHLIL"},
{"BFIx",      "SARIL"},

{"E100",      "ADDAA"},
{"E101",      "SUBAA"},
{"E102",      "MULAA"},
{"E103",      "ANDAA"},
{"E104",      "ORAA"},
{"E105",      "XORAA"},
{"E106",      "SHLAA"},
{"E107",      "SARAA"},
{"E108",      "SHRAA"},
{"E109",      "DIVAA"},
{"E10A",      "MODAA"},
{"E10B",      "NEGAA"},
{"E10C",      "NOTAA"},
{"E10D",      "LNTAA"},
{"E10E",      "CMPAA"},
{"E10FZy",    "LDCA"},

{"E110",      "CVTI2AA"},
{"E111",      "CVTL2AA"},
{"E112",      "CVTF2AA"},
{"E113",      "CVTD2AA"},
{"E114",      "CVTAA2I"},
{"E115",      "CVTAA2L"},
{"E116",      "CVTAA2F"},
{"E117",      "CVTAA2D"},
{"E118",      "CVTAA2IN"},
{"E119",      "CVTAA2LN"},
{"E11A",      "CVTAA2FN"},
{"E11B",      "CVTAA2DN"},
{"E11CKx",    "MVIC"},
{"E11DKx",    "MVLC"},
{"E11EKx",    "MVFC"},
{"E11FKx",    "MVDC"},

{"E120Gx",    "LDTHIS"},
{"E121Gx",    "STTHIS"},
{"E122Gx",    "CALLTH"},
{"E123Ci",    "DCMPIC"},
{"E124CiAx",  "DCJEQIC"},
{"E125CiAx",  "DCJEQIC"},
{"E126CiAx",  "DCJLTIC"},
{"E127CiAx",  "DCJGTIC"},
{"E128CiAx",  "DCJLEIC"},
{"E129CiAx",  "DCJGEIC"},
{"E12ACiAx",  "CJEQIC"},
{"E12BCiAx",  "CJEQIC"},
{"E12CCiAx",  "CJLTIC"},
{"E12DCiAx",  "CJGTIC"},
{"E12ECiAx",  "CJLEIC"},
{"E12FCiAx",  "CJGEIC"},

{"E130",      "LEARI"},
{"E131",      "LEARL"},
{"E132",      "LEARF"},
{"E133",      "LEARD"},
{"E134",      "LEARA"},
{"E135",      "LEARB"},
{"E136",      "LEARS"},
{"E137",      "LEAST"},
{"E138Ci",    "LEARIC"},
{"E139Ci",    "LEARLC"},
{"E13ACi",    "LEARFC"},
{"E13BCi",    "LEARDC"},
{"E13CCi",    "LEARAC"},
{"E13DCi",    "LEARBC"},
{"E13ECi",    "LEARSC"},
{"E13FCi",    "LEASTC"},

{"E140Gj",    "IFXOBJ"},
{"E141ZnIx",  "IFXARR"},
{"E142Gj",    "DFXOBJ"},
{"E143ZnIx",  "DFXARR"},
{"E144",      "LBLCLNP"},
{"E145ZnCx",  "STIXNC"},
{"E146ZnCx",  "DSTIXNC"},
{"E147",      "NEWDYO"},
{"E148Gx",    "SBOS"},
{"E149Gx",    "DSBOS"},
{"E14AGx",    "DSTOS"},
{"E14BGj",    "IFXOBJA"},
{"E14CZi",    "LDDRAL"},
{"E14DZi",    "LDDRVL"},
{"E14EZi",    "STDRAL"},
{"E14FZi",    "STDRVL"},

{"E150",      "LDDRI"},
{"E151",      "LDDRL"},
{"E152",      "LDDRF"},
{"E153",      "LDDRD"},
{"E154",      "STDRI"},
{"E155",      "STDRL"},
{"E156",      "STDRF"},
{"E157",      "STDRD"},
{"E158",      "LDDRSB"},
{"E159",      "LDDRUB"},
{"E15A",      "LDDRSS"},
{"E15B",      "LDDRUS"},
{"E15C",      "LDDRA"},
{"E15D",      "STDRA"},
{"E15E",      "STDRB"},
{"E15F",      "STDRS"},

{"E160",      "DUPAA"},
{"E161",      "DROPAA"},
{"E162",      "DTRAPNULL"},
{"E163Ix",    "LTRAPNULL"},
{"E164",      "LDIXAA"},
{"E165",      "STIXAA"},
{"E166",      "LDIXAI"},
{"E167",      "STIXAI"},
{"E168Gx",    "ISTYPE"},
{"E169Gx",    "DTNISTYPE"},
{"E16AGx",    "DZNISTYPE"},
{"E16BZn",    "AGETI"},
// {"E16CGj",    "LDOSL"},
// {"E16DGj",    "STOSL"},
{"E16E",      "CALLAA"},

// {"E160Zo",      "BINOP"},
// {"E161Zo",      "CMPOP"},
// {"E162ZoIx",    "BINOPL"},
// {"E163ZoIx",    "CMPOPL"},
// {"E164ZoJx",    "BINOPLL"},
// {"E165ZoJx",    "CMPOPLL"},
// {"E166ZoCx",    "BINOPC"},
// {"E167ZoKx",    "BINOPLC"},
// {"E168ZoCx",    "CMPOPC"},
// {"E169ZoKx",    "CMPOPLC"},
// {"E16AZn",      "PUSHN"},
// {"E16BZn",      "POPN"},
// {"E16CZn",      "SWAPN"},
// {"E16DZn",      "ROTLN"},
// {"E16EZn",      "ROTRN"},
// {"E16FZn",      "DUPN"},

{"E170",      "DIVI"},
{"E171",      "DIVL"},
{"E172",      "MODI"},
{"E173",      "MODL"},
{"E174",      "SHRI"},
{"E175",      "SHRL"},
{"E176",      "UDIVI"},
{"E177",      "UDIVL"},
{"E178",      "CVTUI2L"},

// {"E178",      "CMIEQ"},
{"E179",      "CMINE"},
{"E17A",      "CMILT"},
{"E17B",      "CMIGT"},
{"E17C",      "CMILE"},
{"E17D",      "CMIGE"},
{"E17EZiIx",  "LDIXZLL"},
{"E17FZiCi",  "LDIXZLC"},

{"E180",      "RSTIXI"},
{"E181",      "RSTIXL"},
{"E182",      "RSTIXF"},
{"E183",      "RSTIXD"},
{"E184",      "RSTIXA"},
{"E185",      "RSTIXB"},
{"E186",      "RSTIXS"},
{"E187ZiIx",  "STIXZLL"},
{"E188Ci",    "RSTIXIC"},
{"E189Ci",    "RSTIXLC"},
{"E18ACi",    "RSTIXFC"},
{"E18BCi",    "RSTIXDC"},
{"E18CCi",    "RSTIXAC"},
{"E18DCi",    "RSTIXBC"},
{"E18ECi",    "RSTIXSC"},
{"E18FZiCi",  "STIXZLC"},

{"E190",      "CVTAA2ST"},
{"E191",      "CVTST2AA"},
{"E192",      "CATST"},
{"E193",      "CMPST"},
{"E194",      "CMPSST"},
{"E195",      "CMPUST"},
{"E196Zn",    "MATHUFN"},
{"E197Zn",    "MATHBFN"},
{"E198Gj",    "STFXOBJC"},
{"E199Gj",    "STFXOBJD"},
{"E19AZn",    "STIXZAC"},
{"E19BZn",    "DSTIXZAC"},
{"E19CAx",    "JEQNULL"},
{"E19DAx",    "JNENULL"},
{"E19EIxAx",  "JEQNULLL"},
{"E19FIxAx",  "JNENULLL"},

{"E1A0",      "CVTF2HF"},
{"E1A1",      "CVTD2HF"},
{"E1A2",      "CVTHF2F"},
{"E1A3",      "CVTHF2D"},
{"E1A4",      "SWAPI"},
{"E1A5",      "SWAPL"},
{"E1A6",      "SWAPF"},
{"E1A7",      "SWAPD"},
{"E1A8Ci",    "DSTIXIC"},
{"E1A9Ci",    "DSTIXLC"},
{"E1AACi",    "DSTIXFC"},
{"E1ABCi",    "DSTIXDC"},
{"E1ACCi",    "DSTIXAC"},
{"E1ADCi",    "DSTIXBC"},
{"E1AECi",    "DSTIXSC"},
{"E1AF",      "SWAPA"},

{"E1B0",      "CVTA2PTR"},
{"E1B1",      "CVTPTR2A"},
{"E1B2Zi",    "LDDRPL"},
{"E1B3Zi",    "STDRPL"},
{"E1B4Zn",    "DIFFPTR"},
{"E1B5Zn",    "BSWAP"},
{"E1B6Zn",    "LDDRBSW"},
{"E1B7Zn",    "STDRBSW"},

{"E1B8Gj",    "IFXDYV"},
{"E1B9Gj",    "DFXDYV"},
{"E1BAGj",    "IFXLFCN"},
{"E1BBGj",    "DFXLFCN"},
{"E1BCGx",    "MKLFCN"},
{"E1BDZn",    "DSTIXUZ"},
{"E1BEZnIx",  "DSTIXUZL"},
{"E1BFZnIx",  "DSTIXUZRL"},

{"E1C0Kj",    "ADDISLC"},
{"E1C1Kj",    "SUBISLC"},
{"E1C2Ki",    "MULISLC"},
{"E1C3Ki",    "ANDISLC"},
{"E1C4Ki",    "ORISLC"},
{"E1C5Ki",    "XORISLC"},
{"E1C6Ki",    "SHLISLC"},
{"E1C7Ki",    "SARISLC"},
{"E1C8Jx",    "ADDISLL"},
{"E1C9Jx",    "SUBISLL"},
{"E1CAJx",    "MULISLL"},
{"E1CBJx",    "ANDISLL"},
{"E1CCJx",    "ORISLL"},
{"E1CDJx",    "XORISLL"},
{"E1CEJx",    "SHLISLL"},
{"E1CFJx",    "SARISLL"},
{"E1D0Kj",    "ADDILSC"},
{"E1D1Kj",    "SUBILSC"},
{"E1D2Ki",    "MULILSC"},
{"E1D3Ki",    "ANDILSC"},
{"E1D4Ki",    "ORILSC"},
{"E1D5Ki",    "XORILSC"},
{"E1D6Ki",    "SHLILSC"},
{"E1D7Ki",    "SARILSC"},
{"E1D8Jx",    "ADDILSL"},
{"E1D9Jx",    "SUBILSL"},
{"E1DAJx",    "MULILSL"},
{"E1DBJx",    "ANDILSL"},
{"E1DCJx",    "ORILSL"},
{"E1DDJx",    "XORILSL"},
{"E1DEJx",    "SHLILSL"},
{"E1DFJx",    "SARILSL"},
{"E1E0JxCj",  "ADDILLC"},
{"E1E1JxCj",  "SUBILLC"},
{"E1E2JxCi",  "MULILLC"},
{"E1E3JxCi",  "ANDILLC"},
{"E1E4JxCi",  "ORILLC"},
{"E1E5JxCi",  "XORILLC"},
{"E1E6JxCi",  "SHLILLC"},
{"E1E7JxCi",  "SARILLC"},
{"E1E8Lx",    "ADDILLL"},
{"E1E9Lx",    "SUBILLL"},
{"E1EALx",    "MULILLL"},
{"E1EBLx",    "ANDILLL"},
{"E1ECLx",    "ORILLL"},
{"E1EDLx",    "XORILLL"},
{"E1EELx",    "SHLILLL"},
{"E1EFLx",    "SARILLL"},

{"E1F0Ax",    "BEGTRY"},
{"E1F1Ax",    "ENDTRY"},
{"E1F2GxAx",  "CATCH"},
{"E1F3",      "THROW"},
{"E1F4IxAx",  "BEGLEC"},
{"E1F5Ix",    "ENDLEC"},
{"E1F6IxAx",  "ENDLEC2"},
{"E1F7",      "RETHROW"},


/* Vector Block (0x200) */

{"E200Zi",    "LDDINCZLP"},
{"E201Zi",    "STDINCZLP"},
{"E202Zi",    "LDDDECZLP"},
{"E203Zi",    "STDDECZLP"},
{"E204Zi",    "INCZLP"},
{"E205Zi",    "DECZLP"},
{"E206Zi",    "LINCZLP"},
{"E207Zi",    "LDECZLP"},
{"E208Zi",    "LDDINCZLA"},
{"E209Zi",    "STDINCZLA"},
{"E20AZi",    "LDDDECZLA"},
{"E20BZi",    "STDDECZLA"},
{"E20CZi",    "INCZLA"},
{"E20DZi",    "DECZLA"},
{"E20EZi",    "LINCZLA"},
{"E20FZi",    "LDECZLA"},


/* Vector Block (0x400) */
{"E400",      "MKX4I"},
{"E401",      "MKX2L"},
{"E402",      "MKX4F"},
{"E403",      "MKX2D"},
{"E404",      "DUPX"},
{"E405",      "POPX"},
{"E406Ix",    "LDXL"},
{"E407Ix",    "STXL"},
{"E408Ix",    "IFXX"},
{"E409Ix",    "DFXX"},
{"E40AIx",    "IFXXA"},
{"E40BZn",    "LDXX"},
{"E40CZn",    "DLDXX"},
{"E40DZn",    "STXX"},
{"E40EVb",    "BINOPX"},
{"E40FVa",    "UNOPX"},
{"E410",      "CVTI2XL"},
{"E411",      "CVTL2XL"},
{"E412",      "CVTUI2XL"},
{"E413",      "CVTUL2XL"},
{"E414",      "CVTXL2I"},
{"E415",      "CVTXL2L"},
{"E416",      "CVTD2XF"},
{"E417",      "CVTXF2D"},
{"E418",      "CVTXL2XF"},
{"E419",      "CVTXF2XL"},
{"E41A",      "CVTXL2AA"},
{"E41B",      "CVTXF2AA"},
{"E41C",      "CVTAA2XL"},
{"E41D",      "CVTAA2XF"},
{"E420",      "CVTCF2AA"},
{"E421",      "CVTCD2AA"},
{"E422",      "CVTVI2AA"},
{"E423",      "CVTVF2AA"},
{"E424",      "CVTAA2VI"},
{"E425",      "CVTAA2VF"},
{"E426",      "CVTAA2X4I"},
{"E427",      "CVTAA2X4F"},
{"E428",      "CVTX4F2AA"},
{"E429",      "CVTX3F2AA"},
{"E42A",      "CVTXQF2AA"},
{"E42B",      "CVTX2D2AA"},
{"E42C",      "CVTV2F2X2D"},
{"E42D",      "CVTX2D2V2F"},
{"E42E",      "CVTV2F2X4F"},
{"E42F",      "CVTX4F2V2F"},
{"E430",      "MKV2I"},
{"E431",      "MKV2F"},
{"E432",      "SWEV2I"},
{"E433",      "SWEV2F"},
{"E434",      "SWEX2L"},
{"E435",      "SWEX2D"},
{"E436Xx",    "SHUFX4I"},
{"E437Xx",    "SHUFX4F"},

{"E438",      "CMPXL"},
{"E439",      "UCMPXL"},
{"E43A",      "CMPXF"},
{"E43B",      "CMP2XF"},

{"E440",      "LDV2IA"},
{"E441",      "LDV2IB"},
{"E442",      "LDV2FA"},
{"E443",      "LDV2FB"},
{"E444",      "LDX2LA"},
{"E445",      "LDX2LB"},
{"E446",      "LDX2DA"},
{"E447",      "LDX2DB"},
{"E448",      "LDX4IA"},
{"E449",      "LDX4IB"},
{"E44A",      "LDX4IC"},
{"E44B",      "LDX4ID"},
{"E44C",      "LDX4FA"},
{"E44D",      "LDX4FB"},
{"E44E",      "LDX4FC"},
{"E44F",      "LDX4FD"},

{"E450",      "LDIXX"},
{"E451",      "STIXX"},
{"E452Ci",    "LDIXXC"},
{"E453Ci",    "STIXXC"},

{"E454",      "MKX3D"},
{"E455",      "LDX3DA"},
{"E456",      "LDX3DB"},
{"E457",      "LDX3DC"},
{"E458",      "CVTX3F2X3D"},
{"E459",      "CVTX3D2X3F"},

{NULL,        NULL}
};

int bs2c_disasm_decnbl(char *ps)
{
	if((*ps>='0') && (*ps<='9'))
		return(*ps-'0');
	if((*ps>='A') && (*ps<='F'))
		return((*ps-'A')+10);
	if((*ps>='a') && (*ps<='f'))
		return((*ps-'a')+10);
	return(-1);
}

int bs2c_disasm_decbyte(char *ps)
{
	int i0, i1;
	i0=bs2c_disasm_decnbl(ps+0);
	i1=bs2c_disasm_decnbl(ps+1);
	if((i0>=0) && (i1>=0))
		return((i0<<4)|i1);
	return(-1);
}

int bs2c_disasm_matchPrefix(byte *cs, char *pat,
	byte **rcs1, char **rpa1)
{
	char *ps;
	int i;
	
	ps=pat;
	while(*ps)
	{
		i=bs2c_disasm_decbyte(ps);
		if(i>=0)
		{
			if(*cs==i)
			{
				ps+=2;
				cs++;
				continue;
			}
			return(0);
		}
		break;
	}
	*rcs1=cs;
	*rpa1=ps;
	return(1);
}

byte *bs2c_disasm_ReadVLI(byte *cs, int *rv)
{
	u64 uli;
	int i, l;
	
	cs=bs2c_disasm_ReadVLI_Inner(cs, &uli, &l);
	*rv=uli;
	
	return(cs);
}

byte *bs2c_disasm_ReadVLI2(byte *cs, s64 *rv)
{
	u64 uli;
	int i, l;
	
	cs=bs2c_disasm_ReadVLI_Inner(cs, &uli, &l);
	*rv=uli;
	
	return(cs);
}

byte *bs2c_disasm_ReadVLI_Inner(byte *cs, u64 *rv, int *rl)
{
	u64 li;
	int i, j;
	
	i=*cs++;

	if(i<0x80)
	{
		*rv=i;
		*rl=0;
		return(cs);
	}

	if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=1;
		return(cs);
	}

	if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=2;
		return(cs);
	}

	if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		*rl=3;
		return(cs);
	}

	if(i<0xF8)
	{
		i=i&0x07;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

//		i=(i<<8)|(*cs++);
		j=*cs++;
		*rv=(((u64)i)<<8)|j;
		*rl=4;
		return(cs);
	}

	if(i<0xFC)
	{
		i=i&0x03;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		*rv=(((u64)i)<<16)|j;
		*rl=5;
		return(cs);
	}

	if(i<0xFE)
	{
		i=i&0x01;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		*rv=(((u64)i)<<24)|j;
		*rl=6;
		return(cs);
	}

	if(i==0xFE)
	{
		i=*cs++;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		j=*cs++;
		j=(j<<8)|(*cs++);
		j=(j<<8)|(*cs++);
		*rv=(((u64)((u32)i))<<24)|j;
		*rl=7;
		return(cs);
	}

	i=*cs++;
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);

	j=*cs++;
	j=(j<<8)|(*cs++);
	j=(j<<8)|(*cs++);
	j=(j<<8)|(*cs++);
	*rv=(((u64)((u32)i))<<32)|((u32)j);
	*rl=8;
	return(cs);
}

byte *bs2c_disasm_ReadSplitVLI(byte *cs, int *rvh, int *rvl)
{
	int i;
	
	i=*cs++;

	if(i<0x80)
	{
		*rvh=i>>4;
		*rvl=i&15;
		return(cs);
	}

	if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
		*rvh=i>>7;
		*rvl=i&127;
		return(cs);
	}

	if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=i>>11;
		*rvl=i&2047;
		return(cs);
	}

	if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=i>>14;
		*rvl=i&16383;
		return(cs);
	}

	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	*rvh=i>>16;
	*rvl=i&65535;
	return(cs);
}

byte *bs2c_disasm_ReadTripleVLI(byte *cs, int *rvh, int *rvm, int *rvl)
{
	int i;
	
	i=*cs++;

	if(i<0x80)
	{
		*rvh=(i>>4)&7;
		*rvm=(i>>2)&3;
		*rvl=(i   )&3;
		return(cs);
	}

	if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
		*rvh=(i>>9)&31;
		*rvm=(i>>4)&31;
		*rvl=(i   )&15;
		return(cs);
	}

	if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=(i>>14)&127;
		*rvm=(i>> 7)&127;
		*rvl=(i    )&127;
		return(cs);
	}

	if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=(i>>18)&1023;
		*rvm=(i>> 9)& 511;
		*rvl=(i    )& 511;
		return(cs);
	}

	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
	i=(i<<8)|(*cs++);
//	*rvh=i>>16;
//	*rvl=i&65535;
	return(cs);
}


byte *bs2c_disasm_ReadSplit2VLI(byte *cs, int *rvh, s64 *rvl)
{
	u64 li;
	int i;
	
	i=*cs++;

	if(i<0x80)
	{
		*rvh=i>>4;
		*rvl=i&15;
		return(cs);
	}

	if(i<0xC0)
	{
		i=i&0x3F;
		i=(i<<8)|(*cs++);
		*rvh=i>>9;
		*rvl=i&511;
		return(cs);
	}

	if(i<0xE0)
	{
		i=i&0x1F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=i>>13;
		*rvl=i&8191;
		return(cs);
	}

	if(i<0xF0)
	{
		i=i&0x0F;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rvh=i>>20;
		*rvl=i&((1<<20)-1);
		return(cs);
	}

	if(i<0xF8)
	{
		li=i&0x07;
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		*rvh=li>>27;
		*rvl=li&((1<<27)-1);
		return(cs);
	}

	if(i<0xFC)
	{
		li=i&0x03;
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		*rvh=li>>32;
		*rvl=(u32)li;
		return(cs);
	}

	if(i<0xFE)
	{
		li=i&0x01;
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		*rvh=li>>39;
		*rvl=li&((1LL<<39)-1);
		return(cs);
	}

	if(i==0xFE)
	{
		li=*cs++;
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		*rvh=li>>44;
		*rvl=li&((1LL<<44)-1);
		return(cs);
	}

	if(i==0xFF)
	{
		li=*cs++;
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		li=(li<<8)|(*cs++);
		*rvh=li>>52;
		*rvl=li&((1LL<<52)-1);
		return(cs);
	}

	return(cs);
}

byte *bs2c_disasm_ReadFxVLI(byte *cs, double *rv)
{
	double f;
	s64 m;
	int e;
	int i;
	
	cs=bs2c_disasm_ReadSplit2VLI(cs, &e, &m);
	
	e=(e>>1)^((e<<31)>>31);
	m=(m>>1)^((m<<63)>>63);
	
	f=((double)m)*pow(2.0, e);
	*rv=f;
	
//	cs=bs2c_disasm_ReadVLI(cs, &i);
//	i=(i<<8)|(*cs++);
//	*rv=i;
	return(cs);
}

byte *bs2c_disasm_ReadZxVLI(byte *cs, int *rz, int *rv)
{
	int i;
	
	cs=bs2c_disasm_ReadVLI(cs, &i);
	*rz=i&15;
	*rv=i>>4;
	return(cs);
}

#if 0
byte *bs2c_disasm_ReadZxVLI(byte *cs, int *rz, int *rv)
{
	int i;

	i=*cs++;
	*rz=(i>>4)&15;
	
	i=i&15;
	if(i<8)
	{
		*rv=i;
		return(cs);
	}

	if(i<12)
	{
		i=i&3;
		i=(i<<8)|(*cs++);
		*rv=i;
		return(cs);
	}

	if(i<14)
	{
		i=i&1;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		return(cs);
	}
	
	if(i==14)
	{
		i=*cs++;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		*rv=i;
		return(cs);
	}

	if(i==15)
	{
		i=*cs++;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		i=*cs++;
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);
		i=(i<<8)|(*cs++);

		*rv=i;
		return(cs);
	}

	*rv=-1;
	return(cs);
}
#endif

int bs2c_disasm_matchItem(
	BS2CC_CcFrame *frm,
	byte *cs, char *pat,
	byte **rcs1, char **rpa1)
{
	s64 li;
	double f;
	char *ps;
	int i, j;

	ps=pat;
	if(!strncmp(ps, "Ix", 2) ||
		!strncmp(ps, "Jx", 2) ||
		!strncmp(ps, "Lx", 2) ||
		!strncmp(ps, "Gx", 2) ||
		!strncmp(ps, "Gj", 2) ||
		!strncmp(ps, "Ci", 2) ||
		!strncmp(ps, "Cj", 2) ||
		!strncmp(ps, "Ki", 2) ||
		!strncmp(ps, "Kj", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Cx", 2) || !strncmp(ps, "Kx", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		if((frm->zpf==BSVM2_OPZ_FLOAT) || (frm->zpf==BSVM2_OPZ_DOUBLE))
			{ cs=bs2c_disasm_ReadVLI2(cs, &li); }
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zo", 2))
	{
		ps+=2;
		cs++;
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Ax", 2))
	{
		ps+=2;
//		cs+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Fx", 2))
	{
		ps+=2;
//		cs=bs2c_disasm_ReadFxVLI(cs, &i);
//		cs=bs2c_disasm_ReadVLI(cs, &i);
//		cs=bs2c_disasm_ReadVLI(cs, &i);
		cs=bs2c_disasm_ReadFxVLI(cs, &f);
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zx", 2) || !strncmp(ps, "Zy", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadZxVLI(cs, &i, &j);
		if((i==BSVM2_OPZ_FLOAT) || (i==BSVM2_OPZ_DOUBLE))
			{ cs=bs2c_disasm_ReadVLI2(cs, &li); }

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}
	
	if(!strncmp(ps, "Zn", 2) ||
		!strncmp(ps, "Za", 2) || !strncmp(ps, "Zi", 2) ||
		!strncmp(ps, "Va", 2) || !strncmp(ps, "Vb", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadZxVLI(cs, &i, &j);

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}
	
	return(0);
}

int bs2c_disasm_PrintItem(
	BGBDT_MM_ParsePrintInfo *prn,
	BS2CC_CcFrame *frm,
	byte *cs, char *pat,
	byte **rcs1, char **rpa1)
{
	char *pat_zz="ilfdaubsctmvfdx.";
	char *pat_zo[]={
		"+","-","*","&","|","^","<<",">>",
		">>>","/","%","*.", "*^", "/.", "?", "?"};
	char *pat_zc[]={
		"==", "!=", "<", ">", "<=", ">=", "===", "!==",
		"~==", "~!=", "~<", "~>", "~<=", "~>=", "~===", "~!=="};

	char *pat_zv[]={
		"v4i","v2l","v4f","v2d","v2i","v2f","v3i","v3f",
		"ix","ux","fx","?", "v2ih", "v2fh", "?", "?"};
	char *pat_zva[]={
		"neg","rcp","sqrt","rsqrt","len","len2","?","?",
		"?","?","?","?", "?", "?", "?", "?"};
	char *pat_zvb[]={
		"add","sub","mul","div","dot","cross","cmul","cmul2",
		"?","?","?","?", "?", "?", "?", "?"};
	char **pat_zvo;

	double f;
	s64 li;
	char *s;
	char *ps, *ips;
	int i, j, k;

	ps=pat; ips=ps;
	if(!strncmp(ps, "Ix", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);

		BGBDT_MM_PrintPutPrintf(prn, "L%d", i);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Gx", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		
		j=frm->gbltab[i];

		if(!(j&3))
		{
			BGBDT_MM_PrintPutPrintf(prn, "G%d(%s)", i,
				frm->ctx->globals[j>>2]->qname);
		}else if(((j&15)==BSVM2_OPZY_STRASC) ||
			((j&15)==BSVM2_OPZY_STRU8))
		{
			BGBDT_MM_PrintPutPrintf(prn, "Str:%X=", (j>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(j>>4));
		}else if((j&15)==BSVM2_OPZY_STRSYM)
		{
			BGBDT_MM_PrintPutPrintf(prn, "Sym:%X=", (j>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(j>>4));
		}else if((j&15)==BSVM2_OPZY_STRKEY)
		{
			BGBDT_MM_PrintPutPrintf(prn, "Skw:%X=", (j>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(j>>4));
		}else
		{
			BGBDT_MM_PrintPutPrintf(prn, "G%d=%08X", i, j);
		}
		
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Gj", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadSplitVLI(cs, &i, &j);

		BGBDT_MM_PrintPutPrintf(prn, "L%d, ", i);

		k=frm->gbltab[j];
		if(!(k&3))
		{
			BGBDT_MM_PrintPutPrintf(prn, "G%d(%s)", j,
				frm->ctx->globals[k>>2]->qname);
		}else if(((k&15)==BSVM2_OPZY_STRASC) ||
			((k&15)==BSVM2_OPZY_STRU8))
		{
			BGBDT_MM_PrintPutPrintf(prn, "Str:%X=", (k>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
		}else if((k&15)==BSVM2_OPZY_STRSYM)
		{
			BGBDT_MM_PrintPutPrintf(prn, "Sym:%X=", (k>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
		}else if((k&15)==BSVM2_OPZY_STRKEY)
		{
			BGBDT_MM_PrintPutPrintf(prn, "Skw:%X=", (k>>4));
			BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
		}else
		{
			BGBDT_MM_PrintPutPrintf(prn, "G%d=%08X", j, k);
		}
		
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

 	if(!strncmp(ps, "Cx", 2))
	{
		if((frm->zpf==BSVM2_OPZ_FLOAT) || (frm->zpf==BSVM2_OPZ_DOUBLE))
		{
			ps+=2;
			cs=bs2c_disasm_ReadFxVLI(cs, &f);

			BGBDT_MM_PrintPutPrintf(prn, "%f%c", f, pat_zz[frm->zpf]);
			if(*ps)
				BGBDT_MM_PrintPutPrintf(prn, ", ");

			*rcs1=cs;
			*rpa1=ps;
			return(1);
		}
	
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);

		switch(frm->zpf)
		{
		case BSVM2_OPZ_INT:
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_SBYTE:
			i=(i>>1)^((i<<31)>>31);
			break;
		default:
			break;
		}

		BGBDT_MM_PrintPutPrintf(prn, "%d%c", i, pat_zz[frm->zpf]);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

 	if(!strncmp(ps, "Ci", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		i=(i>>1)^((i<<31)>>31);

		BGBDT_MM_PrintPutPrintf(prn, "%d", i);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

 	if(!strncmp(ps, "Cj", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadVLI(cs, &i);

		BGBDT_MM_PrintPutPrintf(prn, "%d", i);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Jx", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadSplitVLI(cs, &i, &j);

		BGBDT_MM_PrintPutPrintf(prn, "L%d, L%d", i, j);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Lx", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadTripleVLI(cs, &i, &j, &k);

		BGBDT_MM_PrintPutPrintf(prn, "L%d, L%d, L%d", i, j, k);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Kx", 2))
	{
		ps+=2;
//		cs=bs2c_disasm_ReadSplit2VLI(cs, &i, &j);
		cs=bs2c_disasm_ReadSplit2VLI(cs, &i, &li);

		if((frm->zpf==BSVM2_OPZ_FLOAT) ||
			(frm->zpf==BSVM2_OPZ_DOUBLE))
		{
			j=li;
			
			cs=bs2c_disasm_ReadVLI2(cs, &li);
			li=(li>>1)^((li<<63)>>63);
			f=li*pow(2.0, j);

			BGBDT_MM_PrintPutPrintf(prn, "%f%c", f, pat_zz[frm->zpf]);
			if(*ps)
				BGBDT_MM_PrintPutPrintf(prn, ", ");

			*rcs1=cs;
			*rpa1=ps;
			return(1);
		}

		switch(frm->zpf)
		{
		case BSVM2_OPZ_INT:
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_SBYTE:
//			j=(j>>1)^((j<<31)>>31);
			li=(li>>1)^((li<<63)>>63);
			BGBDT_MM_PrintPutPrintf(prn, "L%d, %d", i, (s32)li);
			break;
		case BSVM2_OPZ_UINT:
		case BSVM2_OPZ_ULONG:
		case BSVM2_OPZ_USHORT:
		case BSVM2_OPZ_UBYTE:
			BGBDT_MM_PrintPutPrintf(prn, "L%d, %u", i, (u32)li);
			break;
		case BSVM2_OPZ_FLOAT:
		case BSVM2_OPZ_DOUBLE:
			break;
		default:
			break;
		}

//		BGBDT_MM_PrintPutPrintf(prn, "L%d, %d", i, j);
//		BGBDT_MM_PrintPutPrintf(prn, "L%d, %d", i, (int)li);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Ki", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadSplit2VLI(cs, &i, &li);
		li=(li>>1)^((li<<63)>>63);
		BGBDT_MM_PrintPutPrintf(prn, "L%d, %d", i, (s32)li);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Kj", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadSplit2VLI(cs, &i, &li);
//		li=(li>>1)^((li<<63)>>63);
		BGBDT_MM_PrintPutPrintf(prn, "L%d, %d", i, (s32)li);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");
		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zo", 2))
	{
		ps+=2;
		i=*cs++;
		frm->zpf=i>>4;
		frm->zpo=i&15;

		BGBDT_MM_PrintPutPrintf(prn, "%c%s",
			pat_zz[frm->zpf], pat_zo[frm->zpo]);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Va", 2) || !strncmp(ps, "Vb", 2))
	{
		pat_zvo=pat_zva;
		if(!strncmp(ps, "Vb", 2))
			pat_zvo=pat_zvb;

		ps+=2;
//		i=*cs++;
		cs=bs2c_disasm_ReadVLI(cs, &i);
		frm->zpf=i>>4;
		frm->zpo=i&15;
		
		BGBDT_MM_PrintPutPrintf(prn, "%s.%s",
			pat_zvo[frm->zpf], pat_zv[frm->zpo]);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zc", 2))
	{
		ps+=2;
		i=*cs++;
		frm->zpf=i>>4;
		frm->zpo=i&15;

		BGBDT_MM_PrintPutPrintf(prn, "%c%s",
			pat_zz[frm->zpf], pat_zc[frm->zpo]);

		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Ax", 2))
	{
		ps+=2;
//		i=*cs++;
//		i=(i<<8)|(*cs++);

		cs=bs2c_disasm_ReadVLI(cs, &i);

		i=(i>>1)^((i<<31)>>31);

		BGBDT_MM_PrintPutPrintf(prn, "0x%04X", i+(cs-frm->cts));
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Fx", 2))
	{
		ps+=2;
//		cs=bs2c_disasm_ReadFxVLI(cs, &i);
		cs=bs2c_disasm_ReadFxVLI(cs, &f);

		BGBDT_MM_PrintPutPrintf(prn, "%f", f);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}
	
	if(!strncmp(ps, "Zx", 2) || !strncmp(ps, "Zy", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadZxVLI(cs, &i, &j);

		if((i==BSVM2_OPZ_FLOAT) || (i==BSVM2_OPZ_DOUBLE))
		{
			cs=bs2c_disasm_ReadVLI2(cs, &li);
			j=(j>>1)^((j<<31)>>31);
			li=(li>>1)^((li<<63)>>63);
			f=li*pow(2.0, j);

			BGBDT_MM_PrintPutPrintf(prn, "%f%c", f, pat_zz[i]);
			if(*ps)
				BGBDT_MM_PrintPutPrintf(prn, ", ");

			*rcs1=cs;
			*rpa1=ps;
			return(1);
		}

		if(!strncmp(ips, "Zy", 2))
		{
			if((i==BSVM2_OPZ_SBYTE) ||
				(i==BSVM2_OPZ_UBYTE))
			{
				BGBDT_MM_PrintPutPrintf(prn, "Str:%X=", j);
				BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+j);
				if(*ps)
					BGBDT_MM_PrintPutPrintf(prn, ", ");

				*rcs1=cs;
				*rpa1=ps;
				return(1);
			}
		}

		if(i==BSVM2_OPZ_ADDR)
		{
			k=frm->gbltab[j];

			if(!(k&3))
			{
				BGBDT_MM_PrintPutPrintf(prn, "G%d(%s)", j,
					frm->ctx->globals[k>>2]->qname);
			}else if(((k&15)==BSVM2_OPZY_STRASC) ||
				((k&15)==BSVM2_OPZY_STRU8))
			{
				BGBDT_MM_PrintPutPrintf(prn, "Str:%X=", (k>>4));
				BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
			}else if((k&15)==BSVM2_OPZY_STRSYM)
			{
				BGBDT_MM_PrintPutPrintf(prn, "Sym:%X=", (k>>4));
				BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
			}else if((k&15)==BSVM2_OPZY_STRKEY)
			{
				BGBDT_MM_PrintPutPrintf(prn, "Skw:%X=", (k>>4));
				BGBDT_MM_PrintPutString(prn, frm->ctx->strtab+(k>>4));
			}else
			{
				BGBDT_MM_PrintPutPrintf(prn, "G%d=%08X", j, k);
			}

			if(*ps)
				BGBDT_MM_PrintPutPrintf(prn, ", ");

			*rcs1=cs;
			*rpa1=ps;
			return(1);
		}

		if(i==BSVM2_OPZ_VOID)
		{
			switch(j)
			{
			case 0: s="NULL"; break;
			case 1: s="UNDEF"; break;
			case 2: s="TRUE"; break;
			case 3: s="FALSE"; break;
			case 4: s="THIS"; break;
			default: s="?UNK"; break;
			}
			
			BGBDT_MM_PrintPutPrintf(prn, "%s", s);
			if(*ps)
				BGBDT_MM_PrintPutPrintf(prn, ", ");

			*rcs1=cs;
			*rpa1=ps;
			return(1);
		}

		switch(i)
		{
		case BSVM2_OPZ_INT:
		case BSVM2_OPZ_LONG:
		case BSVM2_OPZ_SHORT:
		case BSVM2_OPZ_SBYTE:
		case BSVM2_OPZ_CI_FLOAT:
		case BSVM2_OPZ_CI_DOUBLE:
			j=(j>>1)^((j<<31)>>31);
			break;
		default:
			break;
		}

		BGBDT_MM_PrintPutPrintf(prn, "%d%c",
			j, pat_zz[i]);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zi", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadZxVLI(cs, &i, &j);

		BGBDT_MM_PrintPutPrintf(prn, "L%d%c",
			j, pat_zz[i]);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}

	if(!strncmp(ps, "Zn", 2) || !strncmp(ps, "Za", 2))
	{
		ps+=2;
		cs=bs2c_disasm_ReadZxVLI(cs, &i, &j);

		BGBDT_MM_PrintPutPrintf(prn, "%c:%d",
			pat_zz[i], j);
		if(*ps)
			BGBDT_MM_PrintPutPrintf(prn, ", ");

		*rcs1=cs;
		*rpa1=ps;
		return(1);
	}
	
	return(0);
}

int BS2C_DisAsmOp(
	BGBDT_MM_ParsePrintInfo *prn,
	BS2CC_CompileContext *ctx, BS2CC_CcFrame *frm)
{
	byte *cs0, *cs1, *cs2;
	char *ps1, *ps2;
	int pn;
	int i, j;

	frm->zpf=0;
	frm->zpo=0;

	cs0=frm->cs; pn=-1;
	for(i=0; bs2c_insns[i].frmt; i++)
	{
		if(bs2c_disasm_matchPrefix(
			cs0, bs2c_insns[i].frmt,
			&cs1, &ps1)>0)
			{
				pn=i;
				break;
			}
	}
	
	if(pn<0)
	{
//		BGBDT_MM_PrintPutPrintf(prn, "%04X:%04X <Error>\n",
//			frm->func->gid, (cs0-frm->cts));

		BGBDT_MM_PrintPutPrintf(prn, "%04X:%04X "
				"%02X%02X%02X%02X <Error>\n",
			frm->func->gid, (cs0-frm->cts),
			cs0[0], cs0[1], cs0[2], cs0[3]);
		return(-1);
	}
	
	ps2=ps1; cs2=cs1;
	while(*ps2)
	{
		bs2c_disasm_matchItem(frm, cs2, ps2, &cs2, &ps2);
	}
	
	BGBDT_MM_PrintPutPrintf(prn, "%04X:%04X ",
		frm->func->gid, (cs0-frm->cts));
	
	j=cs2-cs0;
	for(i=0; i<j; i++)
		{ BGBDT_MM_PrintPutPrintf(prn, "%02X", cs0[i]); }
	for(; i<8; i++)
		{ BGBDT_MM_PrintPutPrintf(prn, "  ", cs0[i]); }

	BGBDT_MM_PrintPutPrintf(prn, "%-9s ", bs2c_insns[pn].name);

	ps2=ps1; cs2=cs1;
	while(*ps2)
	{
		bs2c_disasm_PrintItem(prn, frm, cs2, ps2, &cs2, &ps2);
	}

	BGBDT_MM_PrintPutPrintf(prn, "\n");
	
	frm->cs=cs2;
	return(0);
}

void BS2C_DisAsmFuncBody(
	BGBDT_MM_ParsePrintInfo *prn,
	BS2CC_CompileContext *ctx, BS2CC_CcFrame *frm)
{
	int i;

	frm->cs=frm->cts;
	frm->cse=frm->cts+frm->szt;
	
	while(frm->cs<frm->cse)
	{
		i=BS2C_DisAsmOp(prn, ctx, frm);
		if(i<0)break;
	}
}

void BS2C_DisAsmStruct(
	BGBDT_MM_ParsePrintInfo *prn,
	BS2CC_CompileContext *ctx, BS2CC_VarInfo *vari)
{
	BS2CC_VarInfo *vi2;
	char *s;
	int i;
	
	BGBDT_MM_PrintPutPrintf(prn, "%d: %s {\n",
		vari->gid, vari->qname);
	for(i=0; i<vari->nargs; i++)
	{
		vi2=vari->args[i];
		s=vi2->sig;
		if(!s)s="";
		BGBDT_MM_PrintPutPrintf(prn, "%d: %s:%08X(%s)\n",
			vi2->gid,
			vi2->qname, vi2->bty,
			BS2C_GetTypeSig(ctx, vi2->bty));
	}
	BGBDT_MM_PrintPutPrintf(prn, "}\n");
}

BS2VM_API void BS2C_DisAsmFuncs(
	BGBDT_MM_ParsePrintInfo *prn,
	BS2CC_CompileContext *ctx)
{
	BS2CC_VarInfo *vari;
	char *s;
	int i;
	
	for(i=0; i<ctx->nglobals; i++)
	{
		vari=ctx->globals[i];
		if(!vari)
			continue;

		if(!vari->body)
		{
			if(vari->vitype==BS2CC_VITYPE_STRUCT)
				{ BS2C_DisAsmStruct(prn, ctx, vari); continue; }
			if(vari->vitype==BS2CC_VITYPE_CLASS)
				{ BS2C_DisAsmStruct(prn, ctx, vari); continue; }
			if(vari->vitype==BS2CC_VITYPE_IFACE)
				{ BS2C_DisAsmStruct(prn, ctx, vari); continue; }

			if(vari->vitype==BS2CC_VITYPE_GBLVAR)
			{
				s=vari->sig;
				if(!s)s="";
//				BS2C_DisAsmStruct(prn, ctx, vari);
				BGBDT_MM_PrintPutPrintf(prn, "%d: %s:%08X(%s)\n",
					i, vari->qname, vari->bty, s);
				continue;
			}

			BGBDT_MM_PrintPutPrintf(prn, "%d: %s ?\n",
				i, vari->qname);

			continue;
		}
//		if(dtvNullP(vari->bodyExp))
//			continue;

		BGBDT_MM_PrintPutPrintf(prn, "%d: %s%s:\n",
			i, vari->qname, vari->sig);
		BS2C_DisAsmFuncBody(prn, ctx, vari->body);

		BGBDT_MM_PrintPutPrintf(prn, "\tAvg B/tk=%f\n",
			vari->body->szt/(vari->tokcnt+0.00001));
	}
}
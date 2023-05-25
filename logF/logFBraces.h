#pragma once

//Dependices for Main Braces (Open)

std::vector<uint32_t>btoDpndsArr{
    10
};

//Dependices for Main Braces (close)

std::vector<uint32_t>btcDpndsArr{
    10
};

//Braces Types General (Open)

std::vector<std::string> btoStrArr{
    "-<[========","-<========","|}--------",
    "|}========","([========","<========",
    "|)------"," /---==---]"," |=",
    "[#########*!*#########]",
    "|}*****(((>-"
};

void bto0(){}
void bto1(){
    wrtInLog(btoStrArr[0]);
}
void bto2(){
    wrtInLog(btoStrArr[1]);
}
void bto3(){
    wrtInLog(btoStrArr[2]);
}
void bto4(){
    wrtInLog(btoStrArr[3]);
}
void bto5(){
    wrtInLog(btoStrArr[4]);
}
void bto6(){
    wrtInLog(btoStrArr[5]);
}
void bto7(){
    wrtInLog(btoStrArr[6]);
}
void bto8(){
    wrtInLog(btoStrArr[7]);
}
void bto9(){
    wrtInLog(btoStrArr[8]);
}
void bto10(){
    wrtInLog(btoStrArr[9]);
}
void bto11(){
    wrtInLog(btoStrArr[10]);
}

std::array<void(*)(),12>btoArr{
    &bto0,&bto1,&bto2,&bto3,&bto4
   ,&bto5,&bto6,&bto7,&bto8,&bto9
   ,&bto10,&bto11
};

std::vector<uint32_t> btosizesArr{};

void getBTOSizes(size_t size=btoStrArr.size()){
    for(uint32_t i=0;i<size;i++){
        btosizesArr.push_back(btoStrArr[i].size());
    }
}

//Braces Types General (Close)

std::vector<std::string> btcStrArr{
    "========]>-","========>-","--------{|",
    "========{|","========])","========>",
    "------(|"," \\---==---]"," |============]",
    "[#########*!*#########]",
    "-<)))*****{|"
};

bool btcBull{0};

void btc0(){}
void btc1(){
    wrtInLog(btcStrArr[0]);
}
void btc2(){
    wrtInLog(btcStrArr[1]);
}
void btc3(){
    wrtInLog(btcStrArr[2]);
}
void btc4(){
    wrtInLog(btcStrArr[3]);
}
void btc5(){
    wrtInLog(btcStrArr[4]);
}
void btc6(){
    wrtInLog(btcStrArr[5]);
}
void btc7(){
    wrtInLog(btcStrArr[6]);
}
void btc8(){
    wrtInLog(btcStrArr[7]);
}
void btc9(){
    wrtInLog(btcStrArr[8]);
}
void btc10(){
    wrtInLog(btcStrArr[9]);
}
void btc11(){
    wrtInLog(btcStrArr[10]);
}

std::vector<uint32_t> btcsizesArr{};

void getBTCSizes(size_t size=btcStrArr.size()){
    for(uint32_t i=0;i<size;i++){
        btcsizesArr.push_back(btcStrArr[i].size());
    }
}

std::array<void(*)(),12>btcArr{
    &btc0,&btc1,&btc2,&btc3,&btc4
   ,&btc5,&btc6,&btc7,&btc8,&btc9
   ,&btc10,&btc11
};

//Braces Types Enum

void bte0(uint64_t enumCount){}
void bte1(uint64_t enumCount){
    wrtInLog(' ');
}
void bte2(uint64_t enumCount){
    wrtInLog(" >");
}
void bte3(uint64_t enumCount){
    wrtInLog(" |>");
}
void bte4(uint64_t enumCount){
    wrtInLog(" [>");
}
void bte5(uint64_t enumCount){
    wrtInLog(" [");
    wrtInLog(enumCount);
    wrtInLog(">");
}
void bte6(uint64_t enumCount){
    wrtInLog(" |]");
}
void bte7(uint64_t enumCount){
    wrtInLog(" |");
    wrtInLog(enumCount);
    wrtInLog("]");
}
void bte8(uint64_t enumCount){
    wrtInLog(" /");
    wrtInLog(enumCount);
    wrtInLog("}");
}
void bte9(uint64_t enumCount){
    wrtInLog(" >|");
}
std::array<void(*)(uint64_t enumCount),10>bteArr{
    &bte0,&bte1,&bte2,&bte3,&bte4
   ,&bte5,&bte6,&bte7,&bte8,&bte9
};

//Dependices for Enum Braces (Open)

std::vector<uint32_t>bteoDpndsArr{
    8,9
};

//Dependices for Enum Braces (close)

std::vector<uint32_t>btecDpndsArr{
    NULL
};

//Braces Types Variables (Open)

std::vector<std::string> btvoStrArr{
    " "," ["," '"," L"," <","\""
};

void btvo0(){}
void btvo1(){
    wrtInLog(btvoStrArr[0]);
}
void btvo2(){
    wrtInLog(btvoStrArr[1]);
}
void btvo3(){
    wrtInLog(btvoStrArr[2]);
}
void btvo4(){
    wrtInLog(btvoStrArr[3]);
}
void btvo5(){
    wrtInLog(btvoStrArr[4]);
}
void btvo6(){
    wrtInLog(btvoStrArr[5]);
}

std::array<void(*)(),7>btvoArr{
    &btvo0,&btvo1,&btvo2,&btvo3,&btvo4
   ,&btvo5,&btvo6
};

std::vector<uint32_t> btvosizesArr{};

void getBTVOSizes(size_t size=btvoStrArr.size()){
    for(uint32_t i=0;i<size;i++){
        btvosizesArr.push_back(btvoStrArr[i].size());
    }
}

//Braces Types Variables (Close)

std::vector<std::string> btvcStrArr{
    "","]","'","|",">","\""
};

void btvc0(){
}
void btvc1(){
    wrtInLog(btvcStrArr[0]);
}
void btvc2(){
    wrtInLog(btvcStrArr[1]);
}
void btvc3(){
    wrtInLog(btvcStrArr[2]);
}
void btvc4(){
    wrtInLog(btvcStrArr[3]);
}
void btvc5(){
    wrtInLog(btvcStrArr[4]);
}
void btvc6(){
    wrtInLog(btvcStrArr[5]);
}

std::array<void(*)(),7>btvcArr{
    &btvc0,&btvc1,&btvc2,&btvc3,&btvc4
   ,&btvc5,&btvc6
};

std::vector<uint32_t> btvcsizesArr{};

void getBTVCSizes(size_t size=btvcStrArr.size()){
    btvcsizesArr.erase(btvcsizesArr.begin());
    for(uint32_t i=0;i<size;i++){
        btvcsizesArr.push_back(btvcStrArr[i].size());
    }
}

//Braces compile both Brace Types (Open&Close) sizes

inline size_t getBTMaxCount(){
    std::array<size_t,4> candts{
        btoStrArr.size(),
        btcStrArr.size(),
        btvoStrArr.size(),
        btvcStrArr.size()
    };
    size_t max = (*std::max_element(candts.begin(),candts.end()));
    return max;
}

static constexpr void getBTOCSizes(size_t size){
    for(uint32_t i=0;i<size;i++){
        btosizesArr.push_back(btoStrArr[i].size());
        btcsizesArr.push_back(btcStrArr[i].size());
        btvosizesArr.push_back(btvoStrArr[i].size());
        btvcsizesArr.push_back(btvcStrArr[i].size());
    }
}

#pragma once
namespace fs = std::filesystem;
using fsDirP = bool (*)(const std::filesystem::path&);
using EnumList = std::vector<string>;

using logFValue = std::variant<u64,u32,u16,u8,s64,s32,s16,s8,f32,f64,d64,d128>; //not used

u32 _logFWarns{0};
u32 _logFErrs {0};

inline void _logFWarnsIncr(const u32& c = 1){
    _logFWarns += c;
}

inline void _logFErrsIncr(const u32& c = 1){
    _logFErrs += c;
}

struct stat buffer;
string pathDir="logs/";
string pathChk="_VKHQExec.log";
fs::path pathFs=pathDir;
std::ofstream newLog;

string logText;

typedef enum{
    BLCC=0,  GLDC=94, 
    REDC=1,  PNKC=205,
    GRNC=2,  LIMC=82, 
    ORGC=3,  YLWC=11, 
    BLUC=4,  WATC=57,
    MAGC=5,  PRPC=13, 
    CYNC=6,  LBLC=51, 
    WHTC=7,  DPRC=129,
    GRYC=8,  DAQC=23, 
} HGLTtype;

typedef enum{
    OFF=1,
    ON =2,
    RST=3,
} Stmnt;

typedef enum{
    FRST=0x001,
    SCND=0x002,
    THRD=0x004,
    FRTH=0x008,
} BGflags;

HGLTtype stdClr = WHTC;
u32 stdMod = 0;

void logC(u32 color,u32 mod=0,bool ground=0){
    u32 grnd=38;
    if(ground)grnd=48;
    std::cout<<"\033["<<mod<<";"<<grnd<<";5;"<<color<<"m";
}
void logC(Stmnt lock=OFF){
    if(lock&RST)
    std::cout<<"\033["<<stdMod<<";"<<38<<";5;"<<stdClr<<"m";
}

void NULLFUNC(){}
void (*callBackNline)()=&NULLFUNC;

void wrtString(string str){
    std::cout<<str;
    logText+=str;
}
void wrtStringNOCOUT(string str){
    logText+=str;
}
void wrtCharChkr(string str){
    for(char l:str){
        if(l=='\n'){
            std::cout<<'\n';
            logText+='\n';
            callBackNline();
        }else{
            std::cout<<l;
            logText+=l;
        }
    }
}
void wrtCharChkrNOCOUT(string str){
    for(char l:str){
        if(l=='\n'){
            logText+='\n';
            callBackNline();
        }else{
            logText+=l;
        }
    }
}
void (*wrtInLogClbck)(string)=&wrtString;
bool wrtSwitch{0};
#ifndef NDEBUG
void wrtInLog(string str){
    wrtInLogClbck(str);
}
void wrtInLog(char chr){
    wrtInLog(string(1,chr));
}
void wrtInLog(const char* chr){
    wrtInLog(string(chr));
}
void wrtInLog(f32 val){
    wrtInLog(std::to_string(val));
}
template<typename T>
void wrtInLog(T val){
    wrtInLog(std::to_string(val));
}
#else
void wrtInLog(string str){
    return;
};
void wrtInLog(char chr){
    return;
};
void wrtInLog(const char* chr){
    return;
};
void wrtInLog(f32 val){
    return;
};
template<typename T>
void wrtInLog(T val){
    return;
};
#endif

inline string wrtValToStr(f32 val){
    string str = std::to_string(val);
    s pos = str.find_last_not_of('0');
    if(str[pos]!='.')++pos;
    str.erase(pos);
    return str;
}

template<typename T>
inline f32 cstVal(T val){
    return static_cast<f32>(val);
}
inline string cstStr(f32 val){
    return wrtValToStr(val);
}
inline string cstStr(d64 val){
    return wrtValToStr(val);
}
inline string cstStr(d128 val){
    return wrtValToStr(val);
}
template<typename T>
inline string cstStr(T byte){
    return std::to_string(byte);
}

inline string formatSegments(u64 val,u32 count=2){
    if(count<=1){
        return std::to_string(val);
    }
    if(count>2){
        string retStr("");
        u32 accum=count-1;
        for(u32 i=1;i<count;i++){
            if(val>=pow(10,i)){
                accum-=1;
            }else{
                break;
            }
        }
        for(u32 i=0;i<accum;i++)retStr+="0";
        return retStr+std::to_string(val);
    }
    return (val>=10?std::to_string(val):("0"+std::to_string(val)));
}

#ifndef NDEBUG
std::tm* currentDate;
void wrtTime(){
    auto timeNow=std::chrono::high_resolution_clock::now();
    std::time_t timeTNow=std::chrono::high_resolution_clock::to_time_t(timeNow);
    currentDate=std::localtime(&timeTNow);  
    
    u64 year=currentDate->tm_year+1900;
    u64 month=currentDate->tm_mon+1;
    u64 day=currentDate->tm_mday;
    u64 hour=currentDate->tm_hour;
    u64 min=currentDate->tm_min;
    u64 sec=currentDate->tm_sec;
    u64 msec = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    msec%=1000;

    logText+=std::to_string(year);logText+=("/");
    logText+=std::to_string(month);logText+=("/");
    logText+=std::to_string(day);logText+=(" ");
    logText+=formatSegments(hour);logText+=(":");
    logText+=formatSegments(min);logText+=(".");
    logText+=formatSegments(sec);logText+=("'");
    logText+=formatSegments(msec,3);logText+=("|");
}
void wrtFullDate(){
    auto timeNow=std::chrono::high_resolution_clock::now();
    std::time_t timeTNow=std::chrono::high_resolution_clock::to_time_t(timeNow);
    currentDate=std::localtime(&timeTNow);
    u64 year=currentDate->tm_year+1900;
    u64 month=currentDate->tm_mon+1;
    u64 day=currentDate->tm_mday;
    u64 hour=currentDate->tm_hour;
    u64 min=currentDate->tm_min;
    u64 sec=currentDate->tm_sec;
    
    logText+=("\n\n");
    logText+=std::to_string(year);logText+=(" / ");
    logText+=std::to_string(month);logText+=(" / ");
    logText+=std::to_string(day);logText+=(" -- ");
    logText+=std::to_string(hour);logText+=(":");
    logText+=(min>=10?std::to_string(min):("0"+std::to_string(min)));logText+=(".");
    logText+=(sec>=10?std::to_string(sec):("0"+std::to_string(sec)));logText+=('\n');

    logText+=("YYYY / ");
    logText+=(month>=10?"MM / ":"M / ");
    logText+=(day>=10?"DD":"D");
    logText+=(" -- ");
    logText+=(hour>=10?"HH:":"H:");
    logText+=("MM.SS\n");    
}
#else
void wrtTime(){
    return;
};
void wrtFullDate(){
    return;
};
#endif

bool logFNOCOUT{_fs};

#ifndef NDEBUG
void crt_logFile(){
    u32 i=0;
    u32 fileNum{0};
    u32 fileCount{0};
    const u32 find_limit = std::numeric_limits<u32>::max()/10000;
    fileCount=std::count_if(fs::directory_iterator(pathFs),
              fs::directory_iterator{},
              (fsDirP)(fs::is_regular_file));
    wrtSwitch     = 0;
    callBackNline = &NULLFUNC;
    wrtInLogClbck = &wrtString;

    string csh  = "\n[ ";
           csh += std::to_string(_logFWarns) + ":W , " + std::to_string(_logFErrs);
           csh += ":E ]\n";
    wrtInLog(csh);

    wrtFullDate();
    string pathStg = pathDir+std::to_string(i)+pathChk;
    int finded{-1};
    while(fileNum < fileCount&&i<find_limit){
        ++i;
        pathStg = pathDir+std::to_string(i)+pathChk;
        finded=stat(pathStg.c_str(),&buffer);
        if(finded==0)fileNum++;
    }
    
    if(finded==-1){
        i=0;fileNum=0;
    }

    pathStg=pathDir+std::to_string(i+1)+pathChk;
    
    if(!logFNOCOUT)std::cout<<"\nWrite into new path: "<<pathStg<<":\n-------------------------\n";
    newLog.open(pathStg);
    
    ++fileCount;
        
    newLog << logText;
    newLog.close();
}
#else
void crt_logFile(){
    return;
}
#endif

inline void chk_logFdir(){
    if(stat(pathDir.c_str(),&buffer)!=0){
        fs::create_directory(pathDir.c_str());
    };
}

enum msgTypes{
    WRNG=1,
    RERR=2,
    VALD=3,
    IARG=4,
    BALC=5,
};

string logFBool(bool b){
    if(b){
        return "true";
    }else{
        return "false";
    }
}

string space;

void logFRstSpace(){
    space="";
}

string headMsg;
inline void wrtSysMsg(u8 type,string str){
    headMsg="";
    s siz=logText.find_last_not_of(' ');
    if(logText.c_str()[siz]=='\n'){
        siz=logText.find_last_not_of(' ',siz-1);
        if(logText.c_str()[siz]!='\n')headMsg+='\n';
    }else{
        headMsg+="\n\n";
    }

    headMsg+=space;

    if(type==RERR){
        _logFErrsIncr();
        logC(REDC);
        headMsg+="##:RUNTIME_ERROR:##-";
    }else
    if(type==VALD){
        _logFWarnsIncr();
        logC(PNKC);
        headMsg+="##:VALIDATION_LAYERS:##-";
    }else
    if(type==WRNG){
        _logFWarnsIncr();
        logC(ORGC);
        headMsg+="##:WARNING:##-";
    }else
    if(type==IARG){
        _logFErrsIncr();
        logC(YLWC);
        headMsg+="##:INVALID_ARGUMENTS:##-";
    }else
    if(type==BALC){
        _logFErrsIncr();
        logC(LIMC);
        headMsg+="##:BAD_ALLOCATING:##-";
    }

    headMsg+=str+'\n';
    wrtInLog(headMsg);
    logC(stdClr);
    if(type==RERR||
       type==IARG||
       type==BALC){
        crt_logFile();
        throw std::runtime_error(headMsg);
    }
}

#include "logFBraces.h"

void read_bto(u32 n){
    bool btcBull=0;
    return btoArr[n]();
}
void read_btc(u32 n){
    bool btcBull=1;
    return btcArr[n]();
}

void read_btvo(u32 n){
    return btvoArr[n]();
}
void read_btvc(u32 n){
    return btvcArr[n]();
}

void read_bte(u32 n,u64 n2){
    return bteArr[n](n2);
}
bool chk_btDpnds(u32 v,std::vector<u32>& arr){
    for(u32 rv:arr){
        if(rv==v)return true;
    }
    return false;
}

typedef enum{
    NTDS=0x0000,NLNE=0x0010,LOGF=0x0100,STDP=0x1000,
    LTAB=0x0002,CNTR=0x0020,ENUM=0x0200,MLTB=0x2000,
    DTAB=0x0004,EDNL=0x0040,TMNL=0x0400,MDTB=0x4000,
    SPSE=0x0008,VFLT=0x0080,PVAL=0x0800,MSPS=0x8000,
} logFCMD;

std::vector<string> defVect{};

typedef struct{
    u32                  f=NTDS|NLNE;
    u32                  c=1;
    u32                  b=0;
    HGLTtype             h=stdClr;
    u32                  m=0;
    u8                   bg=0;

    string               s="";
    u32                  bs=0;
    HGLTtype             hs=stdClr;
    u32                  ms=0;

    f32                  v=0;
    u32                  bv=0;
    HGLTtype             hv=stdClr;
    u32                  mv=0;

    string               s2="";
    u32                  bs2=0;
    HGLTtype             hs2=stdClr;
    u32                  ms2=0;

    EnumList&            vec=defVect;
    u32                  be=0;
    HGLTtype             he=stdClr;
    u32                  me=0;
    
    u32                  f2=NTDS|NLNE;
    u32                  c2=1;
} logFDet;

logFDet defDet{.f=NTDS|NLNE,.c=1,.b=0,.h=stdClr,.m=0,.bg=0,
               .s="",.bs=0,.hs=stdClr,.ms=0,
               .v=0,.bv=0,.hv=stdClr,.mv=0,
               .s2="",.bs2=0,.hs2=stdClr,.ms2=0,
               .vec=defVect,.be=0,.he=stdClr,.me=0,
               .f2=NTDS|NLNE,.c2=1};

inline void logFMainFlags(u32 flag,u32 count=1){
    if(flag&LOGF){
        if(logFNOCOUT){
            logFNOCOUT=false;
            void (*wrtInLogClbck)(string)=&wrtString;
        }else{
            logFNOCOUT=true;
            void (*wrtInLogClbck)(string)=&wrtStringNOCOUT;
            
        }
    }

    if(flag&TMNL){
        if(!(wrtSwitch)){
            callBackNline=&wrtTime;
            if(logFNOCOUT){
                wrtInLogClbck=&wrtCharChkrNOCOUT;
            }else{
                wrtInLogClbck=&wrtCharChkr;
            }
            wrtSwitch=1;
        }else{
            callBackNline=&NULLFUNC;
            if(logFNOCOUT){
                wrtInLogClbck=&wrtStringNOCOUT;
            }else{
                wrtInLogClbck=&wrtString;
            }
        }
    }

    if(flag&NLNE){
        wrtInLog("\n");
    }

    if(count!=0){
        if(flag&NTDS){
        }else
        if(flag&MLTB){
            for(u32 i=0;i<count*3;i++){
                if(space.size()>=i){
                    space.erase(--space.end());
                }else break;
            }
        }else
        if(flag&MDTB){
            for(u32 i=0;i<count*2;i++){
                if(space.size()>=i){
                    space.erase(--space.end());
                }else break;
            }
        }else
        if(flag&MSPS){
            for(u32 i=0;i<count;i++){
                if(space.size()>=i){
                    space.erase(--space.end());
                }else break;
            }
        }

        if(flag&NTDS){
        }else
        if(flag&LTAB){
            for(u32 i=0;i<count*3;i++){
                space+=' ';
            }
        }else
        if(flag&DTAB){
            for(u32 i=0;i<count*2;i++){
                space+=' ';
            }
        }else
        if(flag&SPSE){
            for(u32 i=0;i<count;i++){
                space+=' ';
            }
        }
    }
}

bool once{_fs};
#ifndef NDEBUG
inline void logF(u32 flag,
                 u32 count,u32 braceType,HGLTtype hglt,u32 mod,u8 bg,
                 string str,u32 braceTypeStr,HGLTtype hgltStr,u32 modStr,
                 f32 val,u32 braceTypeVal,HGLTtype hgltVal,u32 modVal,
                 string str2,u32 braceTypeStr2,HGLTtype hgltStr2,u32 modStr2,
                 EnumList& vector,u32 braceTypeEnum,HGLTtype clrEnum,u32 modEnum,
                 u32 flagEnd,u32 countEnd){
    
    if(once==_fs){
        chk_logFdir();
        getBTOCSizes(getBTMaxCount());
        once=_tr;
    }

    logFMainFlags(flag,count);

    if(!(flag&STDP)&&!(flag&ENUM&&flag&EDNL))wrtInLog(space);

    logC(hglt,mod);
    read_bto(braceType);

    logC(hgltStr,modStr);
    read_btvo(braceTypeStr);

    if(flag&CNTR&&braceType!=0){
        wrtInLog('\n');
        
        u32 size=str.size();
        u32 spaceSize{0};
        size_t btsize=btosizesArr[braceType-1];
        if(size<=btsize){
            spaceSize=space.size()+((btsize-size)/2);
            for(size_t s=0;s<spaceSize;s++){
                wrtInLog(" ");
            }
            wrtInLog(str);
        }
        wrtInLog('\n');
        wrtInLog(space);
    }else{
        wrtInLog(str);
    }
    
    if(!(flag&ENUM)){
        logC(hgltVal,modVal);
        read_btvo(braceTypeVal);

        if(flag&PVAL){
            if(flag&VFLT){
                string stgStrVal=wrtValToStr(val);
                wrtInLog(stgStrVal);
            }else{
                u64 stgVal=static_cast<u64>(val);
                wrtInLog(stgVal);
            }
        }
        
        read_btvc(braceTypeVal);

        logC(hgltStr2,modStr2);
        read_btvo(braceTypeStr2);

        wrtInLog(str2);

        read_btvc(braceTypeStr2);
    }

    logC(hglt);

    if((flag&ENUM&&chk_btDpnds(braceType,bteoDpndsArr)==true)&&!(flag&EDNL)){
        wrtInLog('\n');
    }
    
    if(flag&ENUM){
        u64 enumCount{0};
        for(EnumList::iterator it=vector.begin();it!=vector.end();it++){
            ++enumCount;
            if(count!=0){
                logC(clrEnum);
                if(!(flag&STDP))wrtInLog(space);
            }

            read_bte(braceTypeEnum,enumCount);
            wrtInLog(*it);
            if(!(flag&EDNL&&it==--vector.end()))wrtInLog("\n");
        }
        logC(hglt,mod);
    }
    if(flag&ENUM){
        if(chk_btDpnds(braceTypeEnum,btecDpndsArr)){wrtInLog('\n');}
        else{wrtInLog(space);}
    }

    read_btc(braceType);

    if(flag&ENUM){
        logC(hgltVal,modVal);
        read_btvo(braceTypeVal);

        if(flag&PVAL){
            if(flag&VFLT){
                string stgStrVal=wrtValToStr(val);
                wrtInLog(stgStrVal);
            }else{
                u64 stgVal=static_cast<u64>(val);
                wrtInLog(stgVal);
            }
        }

        read_btvc(braceTypeVal);

        logC(hgltStr2,modStr2);
        read_btvo(braceTypeStr2);

        wrtInLog(str2);

        read_btvc(braceTypeStr2);
    }

    logFMainFlags(flagEnd,countEnd);
}
#else
inline void logF(u32 flag,
                 u32 count,u32 braceType,HGLTtype hglt,u32 mod,u8 bg,
                 string str,u32 braceTypeStr,HGLTtype hgltStr,u32 modStr,
                 f32 val,u32 braceTypeVal,HGLTtype hgltVal,u32 modVal,
                 string str2,u32 braceTypeStr2,HGLTtype hgltStr2,u32 modStr2,
                 EnumList& vector,u32 braceTypeEnum,HGLTtype clrEnum,u32 modEnum,
                 u32 flagEnd,u32 countEnd){
                     return;
                }
#endif

#include "logFOverLds.h"

void logFFlag(u32 flag,u32 spaceCount=1,u32 flag2=NTDS,u32 spaceCount2=1){
    logF({.f=flag,.c=spaceCount,.b=0,.h=stdClr,.m=0,.bg=0,
          .s="",.bs=0,.hs=stdClr,.ms=0,
          .v=0,.bv=0,.hv=stdClr,.mv=0,
          .s2="",.bs2=0,.hs2=stdClr,.ms2=0,
          .vec=defVect,.be=0,.he=stdClr,.me=0,
          .f2=flag2,.c2=spaceCount2});
};

/*
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> retVec;
        
        set<int> setV;
        for(int val:candidates){
            setV.insert(val);
        }

        int c = target/(*setV.begin());
        int have = (*setV.begin())*c;
        int need = target - have;
        if(*setIt-(*setV.begin())==need){
            
        }
        return {{have}};
    }
};*/

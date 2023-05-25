#pragma once
#ifndef NDEBUG

inline void logF(logFDet& det){
    logF(det.f,det.c,
         det.b,det.h,det.m,det.bg,
         det.s,det.bs,det.hs,det.ms,
         det.v,det.bv,det.hv,det.mv,
         det.s2,det.bs2,det.hs2,det.ms2,
         det.vec,det.be,det.he,det.me,
         det.f2,det.c2);
};
inline void logF(const logFDet& det){
    logF(det.f,det.c,
         det.b,det.h,det.m,det.bg,
         det.s,det.bs,det.hs,det.ms,
         det.v,det.bv,det.hv,det.mv,
         det.s2,det.bs2,det.hs2,det.ms2,
         det.vec,det.be,det.he,det.me,
         det.f2,det.c2);
};

inline void logF(string str,u32 flag=STDP,HGLTtype clr1=stdClr,u32 mod1=0,u32 count=0,u32 braceTypeStr=0,u32 flag2=NTDS,u32 count2=1){
    logF(flag,count,
         0,stdClr,stdMod,0,
         str,0,clr1,mod1,
         0,1,stdClr,stdMod,
         "",0,stdClr,stdMod,
         defVect,0,stdClr,stdMod,
         flag2,count2);
}
#else
inline void logF(const logFDet& det){
    return;
}
inline void logF(logFDet& det){
    return;
}
inline void logF(string str,u32 flag=STDP,HGLTtype clr1=stdClr,u32 mod1=0,u32 count=0,u32 braceTypeStr=0,u32 flag2=NTDS,u32 count2=1){
    return;
}
#endif

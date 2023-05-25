using namespace std::chrono;
class Profiler{
public:
    float nv;

    uint64_t count{0};

    std::vector<float> vc;

    float leg{0.0f};
    float sum{0.0f};
    double sumFull{0.0f};

    std::vector<float>::iterator itvc=vc.end();

    uint16_t lim;
    bool repEn,full;
    Profiler(uint16_t limit=1,bool repeatMode=0,bool fullMode=0){
        lim=limit;
        repEn=repeatMode;
        full=fullMode;
        if(repEn){start();}
    }

    high_resolution_clock::time_point t1,t2;
    high_resolution_clock::time_point start(){
        t1=high_resolution_clock::now();
        return t1;
    }
    float end(){
        t2=high_resolution_clock::now();
        nv=(duration_cast<microseconds>(t2 - t1).count())/1000.0f;
        push();
        return nv;
    }
    float step(){
        float ret=end();
        start();
        return ret;
    }

    void push(){
        if(vc.size()==lim){
            leg=*vc.begin();
            vc.erase(vc.begin());
        }
        vc.push_back(nv);

        sum+=nv;
        sum-=leg;

        if(full){
            sumFull+=nv;
            minFull();
            maxFull();
        }
    }

    float time(){
        return nv;
    }
    float min(){
        return (*std::min_element(vc.begin(),vc.end()));
    }
    float max(){
        return (*std::max_element(vc.begin(),vc.end()));
    }
    float avg(){
        return sum/vc.size();
    }
    float avgFull(){
        return sumFull/vc.size();
    }
    float minF,maxF{0};
    float minFull(){
        minF=std::min(minF,nv);
        return minF; 
    }
    float maxFull(){
        maxF=std::max(maxF,nv);
        return maxF; 
    }
    
    void prt(){
        std::cout<<" VectorCache";
        itvc=vc.begin();
        if(vc.size()>1){
            for(int i=0;i<lim&&i<vc.size();++itvc,++i){
                std::cout<<" | "<<*itvc;
            }
        }
        else{
          std::cout<<" | "<<nv;
        }
        std::cout<<" | \n";
    }
};
static Profiler t(100,0,0);

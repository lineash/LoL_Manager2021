//
//  main.cpp
//  LoL_Manager2021
//
//  Created by line.ash on 2021/10/16.
//

#include <iostream>
#include <bangtal>
#include <string>
#include <cstdio>
#include <random>
#define test 0


using namespace bangtal;
using namespace std;

int playerGold = 0;
int userTeam = 0;
int nowRound = 0;

//랜덤
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(13, 28);
uniform_int_distribution<int> pla(0, 15);
uniform_int_distribution<float> topG(0.2f, 3.0f);
uniform_int_distribution<float> jgG(0.7f, 2.5f);
uniform_int_distribution<float> midG(0.8f, 3.5f);
uniform_int_distribution<float> adG(0.6f, 5.0f);
uniform_int_distribution<float> supG(0.4f, 2.0f);


//글로벌 변수 선언
ScenePtr yourTeam, getGold, makeTeam1, makeTeam2, makeTeam3, finTeam;
ScenePtr scene_startPhase;
ScenePtr scene_round[15];

TimerPtr goldTimer;

bool top_can = true;
bool jg_can = true;
bool mid_can = true;
bool ad_can = true;
bool sup_can = true;

//선택 오브젝트 생성
ObjectPtr select_[16];
ObjectPtr showUserTeam;
ObjectPtr top[16], jg[16], mid[16], ad[16], sup[16]; // 0~15
ObjectPtr button_next[16];

string getName;
string playerTeam;
int topN = test;
int jgN = test;
int midN = test;
int adN = test;
int supN = test; //선수 오브젝트 배치 위치계산 등을 위한 카운트

//팀구성 시작
auto scene_top = Scene::create("scene_top", "scene_top.png"); //탑라이너
auto scene_jg = Scene::create("scene_jg", "scene_jg.png");
auto scene_mid = Scene::create("scene_mid", "scene_mid.png");
auto scene_ad = Scene::create("scene_ad", "scene_ad.png");
auto scene_sup = Scene::create("scene_sup", "scene_sup.png");

ScenePtr win = Scene::create("win", "win.png");
ObjectPtr win_button_next = Object::create("button_next.png", win, 565, 50);
ScenePtr lose = Scene::create("lose", "lose.png");
ObjectPtr lose_button_end = Object::create("button_end.png", lose, 565, 50);




//선수 클래스
class Player
{
public:
    string name;
    ObjectPtr player;
    int atk=0, def=0, dex=0, exp=0, gold=0, position=0;
    int num = 0; //position 1=top 2=jg 3=mid 4=ad 5=sup
    Player()
    {
        
    }
    Player(string name, int position):name(name), position(position)
    {
        
    }
    Player(string name, int atk, int def, int dex, int exp, int gold, int position)
    :name(name), atk(atk), def(def), dex(dex), exp(exp), gold(gold), position(position)
    {
        if(position == 1)
        {
            num = topN;
            top[topN] = Object::create(name+".png", scene_top, 75 + 310 * ((topN)/4), 500 - 150 * ((topN)%4), true);
            //player = top[topN];
            topN++;
        }
        else if(position == 2)
        {
            num = jgN;
            jg[jgN] = Object::create(name+".png", scene_jg, 75 + 310 * ((jgN)/4), 500 - 150 * ((jgN)%4), true);
            //player = jg[jgN];
            jgN++;
        }
        else if(position == 3)
        {
            num = midN;
            mid[midN] = Object::create(name+".png", scene_mid, 75 + 310 * ((midN)/4), 500 - 150 * ((midN)%4), true);
            //player = mid[midN];
            midN++;
        }
        else if(position == 4)
        {
            num = adN;
            ad[adN] = Object::create(name+".png", scene_ad, 75 + 310 * ((adN)/4), 500 - 150 * ((adN)%4), true);
            //player = ad[adN];
            adN++;
        }
        else if(position == 5)
        {
            num = supN;
            sup[supN] = Object::create(name+".png", scene_sup, 75 + 310 * ((supN)/4), 500 - 150 * ((supN)%4), true);
            //player = sup[supN];
            supN++;
        }
    }
    
};




//팀 클래스 멤버 초기화를 위한 샘플
Player sample_top("example", 1);
Player sample_jg("example", 2);
Player sample_mid("example",3);
Player sample_ad("example", 4);
Player sample_sup("example", 5);

//팀 클래스
class Team
{
public:
    string teamName;
    Player top = sample_top;
    Player jg = sample_jg;
    Player mid = sample_mid;
    Player ad = sample_ad;
    Player sup = sample_sup;
    Team()
    {
        
    }
    Team(string teamname):teamName(teamname)
    {
        
    }
    void showTeam(ScenePtr scene, int x, int y);
    float score()
    {
        float score_top= (top.atk * ((top.dex*top.exp)/topG(gen)))*1.5 + ((top.def)* ((top.dex*top.exp)/topG(gen)))*1.5;
        float score_jg= (jg.atk * ((jg.dex*jg.exp)/jgG(gen)))*1.8 + ((jg.def)* ((jg.dex*jg.exp)/jgG(gen)))*1.0;
        float score_mid= (mid.atk * ((mid.dex*mid.exp)/midG(gen)))*2.2 + ((mid.def)* ((mid.dex*mid.exp)/midG(gen)))*1.3;
        float score_ad= (ad.atk * ((ad.dex*ad.exp)/adG(gen)))*1.7 + ((ad.def)* ((ad.dex*ad.exp)/adG(gen)))*1.5;
        float score_sup= (sup.atk * ((sup.dex*sup.exp)/supG(gen)))*1.4 + ((sup.def)* ((sup.dex*sup.exp)/supG(gen)))*1.2;
            
        return score_top + score_jg + score_mid + score_ad + score_sup;
    }
};
void Team::showTeam(ScenePtr scene, int x, int y)
    {
        auto show_logo = Object::create(teamName, scene, x-50, y + 600);
        auto show_top = Object::create(top.name + ".png", scene, x, y + 500);
        auto show_jg = Object::create(jg.name + ".png", scene, x, y + 400);
        auto show_mid = Object::create(mid.name + ".png", scene, x, y + 300);
        auto show_ad = Object::create(ad.name + ".png", scene, x, y + 200);
        auto show_sup = Object::create(sup.name + ".png", scene, x, y + 100);
    }
/*
float Team::score()
{
    float score_top= (top.atk * ((top.dex*top.exp)/topG(gen)))*1.5 + ((top.def)* ((top.dex*top.exp)/topG(gen)))*1.5;
    float score_jg= (jg.atk * ((jg.dex*jg.exp)/jgG(gen)))*1.8 + ((jg.def)* ((jg.dex*jg.exp)/jgG(gen)))*1.0;
    float score_mid= (mid.atk * ((mid.dex*mid.exp)/midG(gen)))*2.2 + ((mid.def)* ((mid.dex*mid.exp)/midG(gen)))*1.3;
    float score_ad= (ad.atk * ((ad.dex*ad.exp)/adG(gen)))*1.7 + ((ad.def)* ((ad.dex*ad.exp)/adG(gen)))*1.5;
    float score_sup= (sup.atk * ((sup.dex*sup.exp)/supG(gen)))*1.4 + ((sup.def)* ((sup.dex*sup.exp)/supG(gen)))*1.2;
        
    return score_top + score_jg + score_mid + score_ad + score_sup;
}*/

Team Team[16]; //총 16팀이 참여한다. Team은 0~16

//==============================================================
//==================================================== 플레이어 DB
//==============================================================
//DK
Player khan("khan", 4, 4, 4, 5, 6, 1);
Player canyon("canyon", 5, 4, 4, 3, 6, 2);
Player showmaker("showmaker", 5, 5, 5, 3, 7, 3);
Player ghost("ghost", 2, 4, 3, 3, 4, 4);
Player beryl("beryl", 5, 4, 5, 3, 5, 5); //29골드

//GEN
Player burdol("burdol", 5, 3, 4, 1, 3, 1);
Player clid("clid", 4, 3, 3, 3, 4, 2);
Player bdd("bdd", 4, 4, 3, 4, 5, 3);
Player ruler("ruler", 5, 4, 3, 4, 6, 4);
Player life("life", 4, 3, 3, 2, 3, 5); //21골드

//T1
Player canna("canna", 4, 4, 4, 2, 4, 1);
Player oner("oner", 5, 3, 4, 1, 3, 2);
Player faker("faker", 4, 4, 5, 5, 7, 3);
Player gumayusi("gumayusi", 3, 4, 4, 1, 3, 4);
Player keria("keria", 4, 4, 4, 2, 4, 5); //21골드

//HLE
Player morgan("morgan", 2, 2, 2, 1, 1, 1); //atk, def, dex, exp, gold, pos
Player willer("willer", 2, 2, 3, 1, 2, 2);
Player chovy("chovy", 5, 4, 3, 3, 6, 3);
Player deft("deft", 4, 3, 2, 5, 4, 4);
Player vsta("vsta", 3, 3, 2, 2, 2, 5); //15골드

//EDG
Player flandre("flandre", 4, 4, 4, 2, 4, 1);
Player jiejie("jiejie", 4, 3, 4, 3, 4, 2);
Player scout("scout", 4, 4, 3, 4, 5, 3);
Player viper("viper", 4, 4, 4, 4, 6, 4);
Player meiko("meiko", 4, 4, 4, 3, 4, 5); //23골드

//FPX
Player nuguri("nuguri", 5, 4, 5, 3, 7, 1);
Player tian("tian", 4, 4, 5, 4, 6, 2);
Player doinb("doinb", 3, 4, 5, 5, 6, 3);
Player lwx("lwx", 4, 4, 4, 4, 5, 4);
Player crisp("crisp", 3, 4, 4, 2, 4, 5); //27골드

//RNG
Player xiaohu("xiaohu", 5, 4, 3, 4, 6, 1);
Player wei("wei", 4, 4, 4, 4, 5, 2);
Player cryin("cryin", 4, 4, 3, 4, 5, 3);
Player gala("gala", 5, 4, 3, 2, 5, 4);
Player ming("ming", 4, 3, 5, 4, 5, 5); //26골드

//LNG
Player ale("ale", 4, 3, 3, 2, 4, 1);
Player tarzan("tarzan", 3, 4, 5, 3, 5, 2);
Player icon("icon", 3, 3, 3, 3, 4, 3);
Player light("light", 2, 3, 3, 2, 3, 4);
Player iwandy("iwandy", 3, 2, 3, 2, 3, 5); //19골드

//MAD
Player armut("armut", 4, 3, 3, 3, 4, 1);
Player elyoya("elyoya", 3, 3, 4, 3, 3, 2);
Player humanoid("humanoid", 3, 3, 4, 3, 4, 3);
Player carzzy("carzzy", 3, 3, 3, 2, 3, 4);
Player kaiser("kaiser", 3, 3, 3, 2, 3, 5); //17골드

//RGE
Player odoamne("odoamne", 3, 3, 2, 2, 3, 1);
Player inspired("inspired", 3, 2, 3, 2, 3, 2);
Player larssen("larssen", 3, 3, 4, 3, 4, 3);
Player hans("hans", 5, 4, 4, 4, 6, 4);
Player trymbi("trymbi", 3, 2, 3, 2, 3, 5); //19골드

//FNC
Player adam("adam", 2, 2, 2, 2, 2, 1);
Player bwipo("bwipo", 3, 3, 3, 4, 3, 2);
Player nisqy("nisqy", 2, 2, 3, 2, 3, 3);
Player bean("bean", 2, 2, 2, 1, 2, 4);
Player hylissang("hylissang", 4, 3, 3, 4, 4, 5); //14골드

//100T
Player ssumday("ssumday", 2, 2, 3, 4, 3, 1);
Player closer("closer", 2, 1, 2, 2, 2, 2);
Player abbedagge("abbedagge", 2, 2, 2, 2, 3, 3);
Player fbi("fbi", 2, 1, 2, 2, 2, 4);
Player huhi("huhi", 2, 1, 3, 2, 1, 5); //11골드

//TL
Player alphari("alphari", 3, 2, 2, 2, 3, 1);
Player santorin("santorin", 3, 2, 3, 2, 3, 2);
Player jensen("jensen", 2, 3, 3, 4, 3, 3);
Player tactical("tactical", 3, 2, 2, 2, 3, 4);
Player corejj("corejj", 3, 4, 4, 4, 4, 5); //16골드

//C9
Player fudge("fudge", 2, 3, 3, 2, 3, 1);
Player blaber("blaber", 2, 2, 2, 2, 2, 2);
Player perkz("perkz", 4, 3, 5, 5, 7, 3);
Player zven("zven", 3, 3, 3, 3, 4, 4);
Player vulcan("vulcan", 3, 2, 3, 2, 3, 5); //19골드

//PSG
Player hanabi("hanabi", 2, 2, 3, 3, 2, 1);
Player river("river", 2, 1, 2, 3, 2, 2);
Player maple("maple", 2, 1, 2, 3, 2, 3);
Player unified("unified", 2, 1, 2, 2, 2, 4);
Player kaiwing("kaiwing", 2, 1, 1, 2, 1, 5); //9골드

//DFM
Player evi("evi", 2, 2, 2, 3, 2, 1);
Player steal("steal", 1, 1, 2, 1, 1, 2);
Player aria("aria", 2, 2, 3, 4, 2, 3);
Player yutapon("yutapon", 2, 2, 1, 2, 2, 4);
Player gaeng("gaeng", 1, 1, 2, 1, 1, 5); //8골드

//===========================================================

Player returnPlayer(int position, int num)
{
    if(position == 1)
    {
        if(num == khan.num) return khan;
        else if(num == burdol.num) return burdol;
        else if(num == canna.num) return canna;
        else if(num == morgan.num) return morgan;
        else if(num == flandre.num) return flandre;
        else if(num == nuguri.num) return nuguri;
        else if(num == xiaohu.num) return xiaohu;
        else if(num == ale.num) return ale;
        else if(num == armut.num) return armut;
        else if(num == odoamne.num) return odoamne;
        else if(num == adam.num) return adam;
        else if(num == ssumday.num) return ssumday;
        else if(num == alphari.num) return alphari;
        else if(num == fudge.num) return fudge;
        else if(num == hanabi.num) return hanabi;
        else if(num == evi.num) return evi;
    }
    else if(position == 2)
    {
        if(num == canyon.num) return canyon;
        else if(num == clid.num) return clid;
        else if(num == oner.num) return oner;
        else if(num == willer.num) return willer;
        else if(num == jiejie.num) return jiejie;
        else if(num == tian.num) return tian;
        else if(num == wei.num) return wei;
        else if(num == tarzan.num) return tarzan;
        else if(num == elyoya.num) return elyoya;
        else if(num == inspired.num) return inspired;
        else if(num == bwipo.num) return bwipo;
        else if(num == closer.num) return closer;
        else if(num == santorin.num) return santorin;
        else if(num == blaber.num) return blaber;
        else if(num == river.num) return river;
        else if(num == steal.num) return steal;
    }
    else if(position == 3)
    {
        if(num == showmaker.num) return showmaker;
        else if(num == bdd.num) return bdd;
        else if(num == faker.num) return faker;
        else if(num == chovy.num) return chovy;
        else if(num == scout.num) return scout;
        else if(num == doinb.num) return doinb;
        else if(num == cryin.num) return cryin;
        else if(num == icon.num) return icon;
        else if(num == humanoid.num) return humanoid;
        else if(num == larssen.num) return larssen;
        else if(num == nisqy.num) return nisqy;
        else if(num == abbedagge.num) return abbedagge;
        else if(num == jensen.num) return jensen;
        else if(num == perkz.num) return perkz;
        else if(num == maple.num) return maple;
        else if(num == aria.num) return aria;
    }
    else if(position == 4)
    {
        if(num == ghost.num) return ghost;
        else if(num == ruler.num) return ruler;
        else if(num == gumayusi.num) return gumayusi;
        else if(num == deft.num) return deft;
        else if(num == viper.num) return viper;
        else if(num == lwx.num) return lwx;
        else if(num == gala.num) return gala;
        else if(num == light.num) return light;
        else if(num == carzzy.num) return carzzy;
        else if(num == hans.num) return hans;
        else if(num == bean.num) return bean;
        else if(num == fbi.num) return fbi;
        else if(num == tactical.num) return tactical;
        else if(num == zven.num) return zven;
        else if(num == unified.num) return unified;
        else if(num == yutapon.num) return yutapon;
    }
    else if(position == 5)
    {
        if(num == beryl.num) return beryl;
        else if(num == life.num) return life;
        else if(num == keria.num) return keria;
        else if(num == vsta.num) return vsta;
        else if(num == meiko.num) return meiko;
        else if(num == crisp.num) return crisp;
        else if(num == ming.num) return ming;
        else if(num == iwandy.num) return iwandy;
        else if(num == kaiser.num) return kaiser;
        else if(num == trymbi.num) return trymbi;
        else if(num == hylissang.num) return hylissang;
        else if(num == huhi.num) return huhi;
        else if(num == corejj.num) return corejj;
        else if(num == vulcan.num) return vulcan;
        else if(num == kaiwing.num) return kaiwing;
        else if(num == gaeng.num) return gaeng;
    }
    return sample_ad;
}

//팀 선택 콜백
bool selectTeam_mouseCallback(ObjectPtr object, int x, int y, MouseAction action)
{
    yourTeam = Scene::create("secne_yourTeam", "scene_yourTeam.png");
    for(int i = 0; i<16; i++)
    {
        if(object == select_[i])
        {   //팀 선택 완료 후 보여줌
            userTeam = i;
            char buf[20];
            sprintf(buf, "team_%d.png", i+1);
            Team[userTeam].teamName = buf; //유저팀 정보 저장
            //playerTeam = buf;
            showUserTeam = Object::create(Team[userTeam].teamName, yourTeam, 490, 380);
            auto next = Object::create("button_next.png", yourTeam, 565, 150);
            next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool {
                getGold->enter();
                return true;
            });
            yourTeam->enter();
        }else
        {
            //유저가 선택한 팀이 아니면 ai팀으로 할당된다.
            char t_buf[20];
            sprintf(t_buf, "team_%d.png", i+1);
            Team[i].teamName = t_buf;
            Team[i].top = returnPlayer(1, pla(gen));
            Team[i].jg = returnPlayer(2, pla(gen));
            Team[i].mid = returnPlayer(3, pla(gen));
            Team[i].ad = returnPlayer(4, pla(gen));
            Team[i].sup = returnPlayer(5, pla(gen));
            
        }
    }
    return true;
}


//선수구매 콜백
bool top_selectPlayer_mouseCallback(ObjectPtr object, int x, int y, MouseAction action) //탑
{

    int num= 0;
    //오브젝트 검사
    for(int i = 0; i < 16; i++)
    {
        if(object == top[i]) num = i;
    }
    
    if(playerGold >= returnPlayer(1, num).gold and top_can) //구매가능 검사
    {
        playerGold -= returnPlayer(1, num).gold; //지불
        goldTimer->set(playerGold);
        Team[userTeam].top = returnPlayer(1, num); //영입
        char buf[70];
        sprintf(buf, "선수를 영입했습니다. %d골드가 남았습니다.", playerGold);
        showMessage(Team[userTeam].top.name+buf);
        top_can = false;
        return true;
    }else if (playerGold < returnPlayer(1, num).gold and (top_can == true))
    {
        showMessage("골드가 부족합니다.");
        return true;
    }
    else if (top_can == false)
    {
        showMessage("이미 영입한 포지션입니다.");
        return true;
    }
    
    return true;
}
bool jg_selectPlayer_mouseCallback(ObjectPtr object, int x, int y, MouseAction action)
{

    int num= 0;
    //오브젝트 검사
    for(int i = 0; i < 16; i++)
    {
        if(object == jg[i]) num = i;
    }
    
    if(playerGold >= returnPlayer(2, num).gold and (jg_can == true)) //구매가능 검사
    {
        playerGold -= returnPlayer(2, num).gold; //지불
        goldTimer->set(playerGold);
        Team[userTeam].jg = returnPlayer(2, num); //영입
        char buf[70];
        sprintf(buf, "선수를 영입했습니다. %d골드가 남았습니다.", playerGold);
        showMessage(Team[userTeam].jg.name+buf);
        jg_can = false;
        return true;
    }else if (playerGold < returnPlayer(2, num).gold and (jg_can == true))
    {
        showMessage("골드가 부족합니다.");
        return true;
    }
    else if (jg_can == false)
    {
        showMessage("이미 영입한 포지션입니다.");
        return true;
    }
    
    return true;
}
bool mid_selectPlayer_mouseCallback(ObjectPtr object, int x, int y, MouseAction action)
{

    int num= 0;
    //오브젝트 검사
    for(int i = 0; i < 16; i++)
    {
        if(object == mid[i]) num = i;
    }
    
    if(playerGold >= returnPlayer(3, num).gold and (mid_can == true)) //구매가능 검사
    {
        playerGold -= returnPlayer(3, num).gold; //지불
        goldTimer->set(playerGold);
        Team[userTeam].mid = returnPlayer(3, num); //영입
        char buf[70];
        sprintf(buf, "선수를 영입했습니다. %d골드가 남았습니다.", playerGold);
        showMessage(Team[userTeam].mid.name+buf);
        mid_can = false;
        return true;
    }else if (playerGold < returnPlayer(3, num).gold and (mid_can == true))
    {
        showMessage("골드가 부족합니다.");
        return true;
    }
    else if (mid_can == false)
    {
        showMessage("이미 영입한 포지션입니다.");
        return true;
    }
    
    return true;
}
bool ad_selectPlayer_mouseCallback(ObjectPtr object, int x, int y, MouseAction action)
{

    int num= 0;
    //오브젝트 검사
    for(int i = 0; i < 16; i++)
    {
        if(object == ad[i]) num = i;
    }
    
    if(playerGold >= returnPlayer(4, num).gold and (ad_can == true)) //구매가능 검사
    {
        playerGold -= returnPlayer(4, num).gold; //지불
        goldTimer->set(playerGold);
        Team[userTeam].ad = returnPlayer(4, num); //영입
        char buf[70];
        sprintf(buf, "선수를 영입했습니다. %d골드가 남았습니다.", playerGold);
        showMessage(Team[userTeam].ad.name+buf);
        ad_can = false;
        return true;
    }else if (playerGold < returnPlayer(4, num).gold and (ad_can == true))
    {
        showMessage("골드가 부족합니다.");
        return true;
    }
    else if (ad_can == false)
    {
        showMessage("이미 영입한 포지션입니다.");
        return true;
    }
    
    return true;
}
bool sup_selectPlayer_mouseCallback(ObjectPtr object, int x, int y, MouseAction action)
{

    int num= 0;
    //오브젝트 검사
    for(int i = 0; i < 16; i++)
    {
        if(object == sup[i]) num = i;
    }
    
    if(playerGold >= returnPlayer(5, num).gold and (sup_can == true)) //구매가능 검사
    {
        playerGold -= returnPlayer(5, num).gold; //지불
        goldTimer->set(playerGold);
        Team[userTeam].sup = returnPlayer(5, num); //영입
        char buf[70];
        sprintf(buf, "선수를 영입했습니다. %d골드가 남았습니다.", playerGold);
        showMessage(Team[userTeam].sup.name+buf);
        sup_can = false;
        return true;
    }else if (playerGold < returnPlayer(5, num).gold and (sup_can == true))
    {
        showMessage("골드가 부족합니다.");
        return true;
    }
    else if (sup_can == false)
    {
        showMessage("이미 영입한 포지션입니다.");
        return true;
    }
    
    return true;
}

void playRound(int i)
{
    if(Team[userTeam].score() >= Team[i].score())
    {
        win->enter();
        win_button_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
            scene_round[i+1]->enter();
            return true;
        });
    }
    else
    {
        lose->enter();
        lose_button_end->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
            endGame();
            return true;
        });
    }
}

void makeRound()
{
    int realRound = 1;
    for(int i = 0; i <16; i++)
    {
        if(userTeam != i || true)
        {
            char buf[20];
            sprintf(buf, "round%d.png", realRound++);
            scene_round[i] = Scene::create(buf, buf);
            Team[userTeam].showTeam(scene_round[i], 225, -50);
            Team[i].showTeam(scene_round[i], 850, -50);
            if(i==0)
            {
                scene_round[i]->enter();
            }
            
            button_next[i] = Object::create("button_next.png", scene_round[i], 565, 100);
            button_next[i]->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
                playRound(i);
                return true;
            });
        }
    }

}


void gamePhase(int round)
{
    makeRound();
}


int main(int argc, const char * argv[]) {
    
    //게임옵션설정
    
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
    
    
    //팀 선택씬
    auto scene_selectTeam = Scene::create("scene_selectTeam", "scene_selectTeam.png");
    
    //팀오브젝트를 생성하고 운영할 팀을 선택합니다.
    for(int i = 0; i < 16; i++)
    {
        char buf[20];
        sprintf(buf, "team_%d.png", i+1);
        select_[i] = Object::create(buf, scene_selectTeam, 10 + 310 * ((i)/4), 500 - 150 * ((i)%4) );
        select_[i]-> setOnMouseCallback(selectTeam_mouseCallback);
    }
    
    //시작씬 생성
    auto scene_start = Scene::create("scene_start", "scene_start.png");
    auto button_start = Object::create("button_start.png", scene_start, 30, 100);
    button_start->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)-> bool {
        scene_selectTeam->enter();
        return true;
    });
    
    auto button_end = Object::create("button_end.png", scene_start, 5, 30);
    button_end->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)-> bool{
        endGame();
        return true;
    });
    
    auto bgm = Sound::create("bgm_worlds.mp3");
    bgm->setOnSoundCallback([&](SoundPtr)->bool {
        bgm->play();
        return true;
    });
    
    
    //골드 얻는 씬
    bgm->play();
    getGold = Scene::create("scene_getGold", "scene_getGold.png");
    auto randomGold = Object::create("random.png", getGold, 520, 260);
    auto getGold_next = Object::create("button_next.png", getGold, 565, 100);
    randomGold->setScale(0.5f);
    bool rullet = true;
    randomGold->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        if(rullet)
        {
            playerGold = dis(gen);
            goldTimer = Timer::create(playerGold);
            char buf[50];
            sprintf(buf, "%d 골드를 예산으로 받았습니다.", playerGold);
            showMessage(buf);
            rullet = false;
        }else{
            showMessage("이미 돌리셨습니다. 위의 숫자를 참고하세요");
        }
        return true;
    });
    getGold_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        if(rullet)
        {
            showMessage("룰렛을 먼저 돌리세요");
        }else{
            makeTeam1->enter();
        }
        return true;
    });
    
    //팀구성 소개
    makeTeam1 = Scene::create("scene_makeTeam1", "scene_makeTeam1.png");
    auto m1_next = Object::create("button_next.png", makeTeam1, 565, 50);
    m1_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        makeTeam2->enter();
        return true;
    });
    
    makeTeam2 = Scene::create("scene_makeTeam2", "scene_makeTeam2.png");
    auto m2_next = Object::create("button_next.png", makeTeam2, 565, 50);
    m2_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        makeTeam3->enter();
        return true;
    });
    
    makeTeam3 = Scene::create("scene_makeTeam3", "scene_makeTeam3.png");
    auto m3_next = Object::create("button_next.png", makeTeam3, 565, 50);
    m3_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        scene_top->enter();
        return true;
    });
    
    //탑 선수를 선택합니다.
    for(int i = test; i < 16; i++)
    {
        top[i]-> setOnMouseCallback(top_selectPlayer_mouseCallback);
    }
    auto top_button = Object::create("button_next.png", scene_top, 1050, 600);
    top_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        scene_jg->enter();
        return true;
    });
    //정글 선수를 선택합니다.
    for(int i = test; i < 16; i++)
    {
        jg[i]-> setOnMouseCallback(jg_selectPlayer_mouseCallback);
    }
    auto jg_button = Object::create("button_next.png", scene_jg, 1050, 600);
    jg_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        scene_mid->enter();
        return true;
    });
    //미드 선수를 선택합니다.
    for(int i = test; i < 16; i++)
    {
        mid[i]-> setOnMouseCallback(mid_selectPlayer_mouseCallback);
    }
    auto mid_button = Object::create("button_next.png", scene_mid, 1050, 600);
    mid_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        scene_ad->enter();
        return true;
    });
    //원딜 선수를 선택합니다.
    for(int i = test; i < 16; i++)
    {
        ad[i]-> setOnMouseCallback(ad_selectPlayer_mouseCallback);
    }
    auto ad_button = Object::create("button_next.png", scene_ad, 1050, 600);
    ad_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        scene_sup->enter();
        return true;
    });
    //서폿 선수를 선택합니다.
    for(int i = test; i < 16; i++)
    {
        sup[i]-> setOnMouseCallback(sup_selectPlayer_mouseCallback);
    }
    auto sup_button = Object::create("button_next.png", scene_sup, 1050, 600);
    sup_button->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
        if(Team[userTeam].top.name == "" || Team[userTeam].jg.name == "" || Team[userTeam].mid.name == "" || Team[userTeam].ad.name == "" || Team[userTeam].sup.name =="") //영입하지 못한 팀원이 있을 경우
        {
            auto scene_selectfail = Scene::create("scene_selectfail", "scene_selectfail.png");
            scene_selectfail->enter();
            auto button_end = Object::create("button_end.png", scene_selectfail, 565, 100);
            button_end -> setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
                endGame(); //첫번쨰 엔딩
                return true;
            });
        }else{ //팀 완성
            finTeam = Scene::create("scene_finTeam", "scene_finTeam.png");
            finTeam->enter();
            //showUserTeam = Object::create(Team[userTeam].teamName, finTeam, 1000, 600);
            Team[userTeam].showTeam(finTeam, 850, -50);
            auto button_next = Object::create("button_next.png", finTeam, 400, 100);
            button_next->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)->bool{
                scene_startPhase = Scene::create("scene_startPhase", "scene_startPhase.png");
                scene_startPhase->enter();
                auto button_phase1 = Object::create("button_next.png", scene_startPhase, 740, 100);
                button_phase1->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)-> bool {
                    gamePhase(0);
                    return true;
                });
                return true;
            });
        }
        return true;
    });

    
        
    
    
    
    
    //게임시작
    startGame(scene_start);
    
}

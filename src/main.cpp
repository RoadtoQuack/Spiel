//#include <CLI/CLI.hpp>
//#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <unistd.h>

// Forward declaration of Mobs struct
namespace local
{
    struct Mobs;
    struct Helden;
}
// Definition of Helden struct
namespace local{



    struct Helden
    {
        std::string Name;
        std::string Klasse;
        std::string Rasse;
        int hp;
        int lvl;
        int attackdmg;
        int magicdmg;
       
    
    public:
    Helden( const std::string& Name,
            const std::string& Klasse, 
            const std::string& Rasse, 
            int hp, 
            int lvl,
            int attackdmg, 
            int magicdmg) :
            Name(Name),
            Klasse(Klasse),
            Rasse(Rasse),
            hp(hp),
            lvl(lvl),
            attackdmg(attackdmg),
            magicdmg(magicdmg)
            {

            }
            void print() const{
                std::cout << Name << " | Level:" << lvl << " | " << Klasse << " | " << Rasse << std::endl;
                std::cout << "Health        : " << hp << std::endl;
                std::cout << "Attack-Damage : " << attackdmg << std::endl;
                std::cout << "Magic-Power   : " << magicdmg << std::endl;
                std::cout << "-----------------------------------\n\n";
            }
                    void Attack(Mobs& mob);
                    void MagicAttack(Mobs& mob);
     //       void Attack(Mobs& mob){
    //            int damage = attackdmg;

   //            mob.getDamage(damage);
    //        }
   //         void MagicAttack(Mobs& mob){
    //            int damage = magicdmg;

    //            mob.getDamage(damage);
   //         }
            void lvlup(){

            }
            void getDamage(int damage){
                hp = hp - damage;
                if (hp <= 0){
                    Tod();
                }

            }
            void Heal(){

            }
            void Tod(){

            }
            void getHealth(){

            }

    };
    struct Mobs
    {
        std::string Name;
        std::string Rasse;
        int hp;
        int lvl;
        int attackdmg;
        int magicdmg;
    
    public:
    Mobs( const std::string& Name, 
            const std::string& Rasse, 
            int hp, 
            int lvl,
            int attackdmg, 
            int magicdmg) :
            Name(Name),
            Rasse(Rasse),
            hp(hp),
            attackdmg(attackdmg),
            magicdmg(magicdmg){}
            

            
            void print() const{
                std::cout << Name << " | Level" << lvl << " | " << Rasse << std::endl;
                std::cout << "Health        : " << hp << std::endl;
                std::cout << "Attack-Damage : " << attackdmg << std::endl;
                std::cout << "Magic-Power   : " << magicdmg << std::endl;
                std::cout << "-----------------------------------\n\n";
            }
                     void Attack(Helden& held);
                    void MagicAttack(Helden& held);
   //         void Attack(Helden& held){
//int damage = attackdmg;

   //          held.getDamage(damage);
   //         }
 //           void MagicAttack(Helden& held){
   //             int damage = magicdmg;

  //              held.getDamage(damage);
   //         }
            void getDamage(int damage){
                hp = hp - damage;
                if (hp <= 0){
                    Tod();
                }
            }
            
            void Tod(){
               //Kampfende
            }

    };
}
// Implementation of Helden methods using Mobs
namespace local{
                void Helden::Attack(Mobs& mob){
                int damage = attackdmg;

                mob.getDamage(damage);
            }
            void Helden::MagicAttack(Mobs& mob){
                int damage = magicdmg;

                mob.getDamage(damage);
            }
}
// Implementation of Mobs methods using Helden
namespace local{
            void Mobs::Attack(Helden& held){
             int damage = attackdmg;

             held.getDamage(damage);
            }
            void Mobs::MagicAttack(Helden& held){
                int damage = magicdmg;

                held.getDamage(damage);
            }
}
namespace local{


    struct StateEngine_T{
        private: 

        using State_T = enum{
            StartScreen_S,
            showMainMenu_S,
            Kampf_S,
            showKampf_S,
            Besiegt_S,
            showComsumables_S,
            showCharacter_S,
            showShop_S,
            exit_S
        };
        State_T nextState_;
        std::vector<Helden> Held{};
        double coins;
        //nlohmann::json db;

        void printMM(){
            system("clear");

            std::cout << "Final Slug Metal Fantasy\n";
            std::cout << "-------------------------------\n\n";

            std::cout << "Kämpfen\n";
            std::cout << "Verbrauchsgegenstände\n";
            std::cout << "Character Übersicht\n";
            std::cout << "Shop\n";
            std::cout << "Beenden\n";
        }
        void printKampfMenu(){
            std::cout << "Kampfoptionen\n";
            std::cout << "---------------------\n";
            std::cout << "Gegner mit Physical Damage angreifen\n";
            std::cout << "Gegner mit Magical Damage angreifen\n";
            std::cout << "Fliehen\n";
        }

        int exit(){
            std::cout << "Save to file? (y/n): ";
            char c{};
            std::cin >> c;
            if(c == 'y' || c == 'Y'){
                std::string save_path{};
                std::cout << "Save file as: ";
                std::cin >> save_path;

                std::ofstream save_as{save_path};
              //  save_as << db.dump();
                save_as.close();
            }

            return 0;
        }
        State_T showKampf(){
                return showMainMenu_S;
        }

        State_T Kampf(Helden& held){
            system("clear");
            Mobs Goblin("Izolb", "Goblin", 100, 1, 5, 0);

            printKampfMenu();

            int choice{};
            std::cin >> choice;

            switch(choice){
                case 1: 
                 held.Attack(Goblin);
                if(Goblin.hp > 0 ){
                    Goblin.Attack(held);
                    return showKampf_S;
                }else
                {
                    return Besiegt_S;
                }
                break; 
                case 2:
                held.MagicAttack(Goblin);
                if(Goblin.hp > 0 ){
                    Goblin.Attack(held);
                return showKampf_S;
                }else
                {
                    return Besiegt_S;
                }
                break;
                case 3:
                system("clear");
                std::cout << Goblin.Name << ": Du Feigling\n";
                usleep(500 *1000); //Microsekunden
                std::cout << "Erfolgreich Entkommen\n";
                usleep(1000 *1000); //Mikrosekunden
                return showMainMenu_S;
                break;

            }
        }



    };
};  

int main(){
    std::cout << "Hallo" << std::endl;
    std::cout << "Hallo" << std::endl;
    std::cout << "Hallo" << std::endl;
    std::cout << "Hallo" << std::endl;
    return 0;
}

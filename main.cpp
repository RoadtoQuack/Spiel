#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>

namespace local
{
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
            magicdmg(magicdmg)
            {

            }
            void print() const{
                std::cout << Name << " | Level" << lvl << " | " << Rasse << std::endl;
                std::cout << "Health        : " << hp << std::endl;
                std::cout << "Attack-Damage : " << attackdmg << std::endl;
                std::cout << "Magic-Power   : " << magicdmg << std::endl;
                std::cout << "-----------------------------------\n\n";
            }
    };  

};
int main(){
    return 0;
}

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <unistd.h>

//g++ -I/root/Spiel/build/_deps/nlohmann_json-src/include -I/root/Spiel/build/_deps/cli11-src/include src/main.cpp

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
        int maxhp;
        int exp;
        int lvl;
        int attackdmg;
        int magicdmg;
       
    
    public:
    Helden( const std::string& Name,
            const std::string& Klasse, 
            const std::string& Rasse, 
            int hp,
            int maxhp,
            int exp, 
            int lvl,
            int attackdmg, 
            int magicdmg) :
            Name(Name),
            Klasse(Klasse),
            Rasse(Rasse),
            hp(hp),
            maxhp(maxhp),
            exp(exp),
            lvl(lvl),
            attackdmg(attackdmg),
            magicdmg(magicdmg)
            {

            }
            void print() const{
                std::cout << Name << " | Level:" << lvl << " | " << Klasse << " | " << Rasse << std::endl;
                std::cout << "EXP           : " << exp << "/2000" << std::endl;
                std::cout << "Health        : " << hp << "/" << maxhp << std::endl;
                std::cout << "Attack-Damage : " << attackdmg << std::endl;
                std::cout << "Magic-Power   : " << magicdmg << std::endl;
                std::cout << "-----------------------------------\n\n";
            }
            //Forward Declaration der Funktionen
            void Attack(Mobs& mob);
            void MagicAttack(Mobs& mob);

   //Level up mit Stats upgrade
            void lvlup(){
                lvl += 1;
                attackdmg = attackdmg + 10;
                magicdmg = magicdmg + 5;
                maxhp = maxhp + 10;
                hp += 10;
                exp = exp - 2000;
            }
            void getEXP(int xp){
                exp = exp + xp;
                if (exp >= 2000)
                {
                    lvlup();
                }
            }
            void getDamage(int damage){
                hp = hp - damage;
                if (hp <= 0){
                }

            }
            void getHealth(int potion){//Einsatz eines Heiltrankes
                if(hp + potion <= maxhp)
                    {
                        hp += potion;
                        std::cout << "Die HP von " << Name << " wurden um " << potion << " geheilt" << std::endl;
                    }else if(hp = maxhp){
                        std::cout << "Die HP von " << Name << " sind schon voll" << std::endl;
                    }else if (hp + potion > maxhp & hp < maxhp){
                        std::cout << "Die HP von " << Name << " wurden um " << maxhp - hp  << " geheilt" << std::endl;
                        hp = maxhp;
                    }
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
                std::cout << "-----------------------------------\n";
            }
            //Forward Declaration der Funktionen
            void Attack(Helden& held);
            void MagicAttack(Helden& held);

            void getDamage(int damage){
                hp -= damage;
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
        std::vector<Mobs> Mob{};
        int coins = 0;
        int numbpotion = 0;
        nlohmann::json db;
        

        void printMM(){//Ausgabe des Mainmenüs
            std::cout << "Final Slug Metal Fantasy\n";
            std::cout << "-------------------------------\n\n";

            std::cout << "1. Kämpfen\n";
            std::cout << "2. Verbrauchsgegenstände\n";
            std::cout << "3. Character Übersicht\n";
            std::cout << "4. Shop\n";
            std::cout << "5. Beenden\n";
        }
        void printKampfMenu(){//Ausgabe des Kampfmenüs
            std::cout << "Kampfoptionen\n";
            std::cout << "--------------------------------\n";
            std::cout << "1. Gegner mit Physical Damage angreifen\n";
            std::cout << "2. Gegner mit Magical Damage angreifen\n";
            std::cout << "3. Fliehen\n";
        }
        void printShop(){//Ausgabe des Shops
            std::cout << "Shop" << std::endl;
            std::cout << "--------------------------------------\n\n";
            std::cout << "1. Gegenstände einkaufen\n";
            std::cout << "2. Gegenstände verkaufen\n";
            std::cout << "3. Zurück zum Menü\n";
        }

        int exit(){
            //Aktualisierung der Objekte in der Json File
            for(auto& held: Held){
                if (!db["Held"].empty()) {
                db["Held"][0]["Name"] = held.Name; 
                db["Held"][0]["Klasse"] = held.Klasse;
                db["Held"][0]["Rasse"] = held.Rasse;
                db["Held"][0]["Health"] = held.hp;// Beispiel für die Änderung des Health-Werts
                db["Held"][0]["MaxHealth"] = held.maxhp;
                db["Held"][0]["EXP-Punkte"] = held.exp;
                db["Held"][0]["Level"] = held.lvl;
                db["Held"][0]["AttackDamage"] = held.attackdmg;
                db["Held"][0]["MagicDamage"] = held.magicdmg;
                }
            }
            if(!db["coins"].empty()){
                db["coins"] = coins;
            }
            if(!db["numbpotion"].empty()){
                db["numbpotion"] = numbpotion;
            }

            //Speichern der Datei
            std::cout << "Save to file? (y/n): ";
            char c{};
            std::cin >> c;
            if(c == 'y' || c == 'Y'){
                std::string save_path{};
                std::cout << "Save file as: ";
                std::cin >> save_path;

                std::ofstream save_as{save_path};
                save_as << db.dump();
                save_as.close();
            }

            return 0;
        }
         State_T showConsumables(){

            while (true){
            system("clear");

            for( auto& held : Held){
            std::cout << "Verbrauchsgegenstände\n";
            std::cout << "-----------------------------\n\n";
            std::cout << held.Name << " | Level: " << held.lvl << " | " << held.Klasse << " | " << held.Rasse << std::endl;
            std::cout << "Health        : " << held.hp << "/" << held.maxhp << std::endl;


            std::cout << "Du hast " << numbpotion << " Heiltränke" << std::endl; 
            std::cout << std::endl;
            std::cout << "Will du einen Heiltrank verwenden? (y/n)" << std::endl;
            int potion = 15;                
            char ch = std::cin.get();
            if ( ch == 'y' || ch == 'Y'){
                if(numbpotion > 0){
                    system("clear");
                    int temphp = held.hp;
                    held.getHealth(potion);
                    if(temphp < held.hp){
                        numbpotion -= 1;
                    }
                    usleep(1000 * 1000);
                }else{
                    system("clear");
                    std::cout << "Du hast nicht genug Heiltränke" << std::endl;
                    usleep(1000 * 1000);
                }
                }

            if (ch == 'n' || ch == 'N'){
                return showMainMenu_S;
                break;
            }   
        }    
    }
            return showMainMenu_S;
}

        State_T showCharacter(){
                system("clear");
                for (const auto& held : Held) {
                    held.print();
            }
                std::cout << "Drücke B um zurück ins Hauptmenü zugehen\n";

                while (true) {
                char ch = std::cin.get(); // Liest ein einzelnes Zeichen von der Eingabe

                if (ch == 'b' || ch == 'B') {
               // Eingabetaste (Enter) erkannt
                std::cout << "Eingabetaste (Enter) wurde gedrückt." << std::endl;
                break; // Endet die Schleife nachdem die Eingabetaste erkannt wurde
                }

            }
            return showMainMenu_S;
        }

        State_T showShop(){ 
            system("clear");
            printShop(); 
            int price = 100;
            
            while(true){
                int Wahl;
                std::cin >> Wahl;
                if(Wahl == 1){
                    
                    while(true){
                    system("clear");
                    std::cout << "Shop\n";
                    std::cout << "--------------------------------------------\n";
                    std::cout << "coins   : " << coins << std::endl; 
                    std::cout << "Potions : " << numbpotion << std::endl;
                    std::cout << "\nWelche Gegenstände willst du kaufen?\n\n";
                    std::cout << "1. Heilpotions" << std::endl;
                    std::cout << "2. Zurück\n";
                    int input{};
                    std::cin >> input;
                    if(input == 1 ){
                        if(coins >= price){
                            system("clear");
                            numbpotion += 1;
                            coins -= price;
                            std::cout << "Du hast 1 Potion für " << price << " coins gekauft\n";
                            std::cout << "Rest coins: " << coins << std::endl;
                            usleep(1000 * 1000);
                        }else{
                            system("clear");
                            std::cout << "Du hast nicht genügend coins\n";
                            usleep(2000 * 1000);
                        }
                    }else if (input == 2){        
                    return showShop_S;
                    }   
                                        
                    }
                }else if(Wahl == 2){
                    
                    while(true){
                    system("clear");
                    std::cout << "Shop\n";
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Welche Gegenstände willst du verkaufen?\n\n";
                    std::cout << "1. Heilpotions : " << numbpotion << std::endl;
                    std::cout << "2. Zurück\n";
                    int input{};
                    std::cin >> input;
                    if(input == 1 ){
                        if(numbpotion > 0){
                            system("clear");
                            numbpotion -= 1;
                            coins += (price*0.8);
                            std::cout << "Du hast 1 Potion für " << price*0.8 << " coins verkauft\n";
                            std::cout << "Du Besitzt jetzt " << coins << " coins\n";
                            usleep(1000 * 1000);
                        }else{
                            system("clear");
                            std::cout << "Du hast keine Potions\n";
                            usleep(2000 * 1000);
                        }
                    }else if (input == 2){        
                    return showShop_S;
                    }   
                                        
                    }
                }else if(Wahl == 3){
                    break;
                }
            }
            return showMainMenu_S;
        }

        State_T showMainMenu(){
            system("clear");
            printMM();

            int choice{};
            std::cin >> choice;

            switch(choice){
                case 1:
                return Kampf_S;
                break;
                case 2:
                return showComsumables_S;
                break;
                case 3:
                return showCharacter_S;
                break;
                case 4:
                return showShop_S;
                break;
                case 5:
                return exit_S;
                break;
                default:
                return showMainMenu_S;
                break;
            }
        }
        State_T showKampf(){
            system("clear");
                 for (const auto& held : Held) {
                held.print();
            }
                std::cout << "Enemy:" << std::endl;
                for (const auto& mobs : Mob){
                mobs.print();
            }
                usleep(2000 *1000);
                return Kampf_S;
        }

        State_T Besiegt(){
                std::cout << "Dein Gegner wurde besiegt\n";
                std::cout << "Du erhälst 1000EP und 200 coins\n";
                coins += 200;

                for (auto& held : Held) {
                held.getEXP(1000);
                usleep(1000 * 1000);
                 }
                 Mob.pop_back();
                return showMainMenu_S;

        }
        State_T StartScreen(){
            system("clear");
            std::cout << "Final Slug Metal Fantasy\n\n";
            std::cout << "Press Enter to Start\n";
                
            while (true) {
                char ch = std::cin.get(); // Liest ein einzelnes Zeichen von der Eingabe

                if (ch == '\n') {
               // Eingabetaste (Enter) erkannt
                std::cout << "Eingabetaste (Enter) wurde gedrückt." << std::endl;
                break; // Endet die Schleife nachdem die Eingabetaste erkannt wurde
                }
            }

            if(Held.empty()){
                system("clear");
                std::string Name, Klasse, Rasse;
                int HP = 100;
                int AttackDamage = 15;
                int MagicDamage = 5;

                std::cout << "Erstelle einen neuen Character\n\n";
                std::cout << "Name: ";
                std::cin >> Name;
                std::cout << "Klasse: ";
                std::cin >> Klasse;
                std::cout << "Rasse: ";
                std::cin >> Rasse;
                std::cout << "MaxHP: 100\n";
                std::cout << "AttackDamage: 15\n";
                std::cout << "MagicDamage: 15\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear input buffer

                Helden newHeld(Name, Klasse, Rasse, HP, HP, 0, 1, AttackDamage, MagicDamage);
                Held.push_back(newHeld);

                nlohmann::json HeldenJson = {
                    {"Name", Name},
                    {"Klasse", Klasse},
                    {"Rasse", Rasse},
                    {"Health", HP},
                    {"MaxHealth", HP},
                    {"EXP-Punkte", 0},
                    {"Level", 1},
                    {"AttackDamage", AttackDamage},
                    {"MagicDamage", MagicDamage}
                };
                db["Held"].push_back(HeldenJson);
            }
            return showMainMenu_S;
        }

        State_T Kampf(){
            system("clear");
            
            if(Mob.empty()){
                Mobs newMob("Izolb", "Goblin", 100, 1, 5, 0); //Erweiterung mit Randomnumbers
                Mob.push_back(newMob);
            }
            Mobs newMob = Mob[0];
            printKampfMenu();
            for(auto& held : Held){
            
            
            int choice{};
            std::cin >> choice;

            switch(choice){
                case 1: 
                 held.Attack(newMob);
                 Mob[0] = newMob;
                if(newMob.hp > 0 ){
                    newMob.Attack(held);
                    return showKampf_S;
                }else
                {
                    return Besiegt_S;
                }
                break; 
                case 2:
                held.MagicAttack(newMob);
                Mob[0] = newMob;
                if(newMob.hp > 0 ){
                    newMob.Attack(held);
                return showKampf_S;
                }else
                {
                    return Besiegt_S;
                }
                break;
                case 3:
                system("clear");
                std::cout << newMob.Name << ": Du Feigling\n";
                usleep(500 *1000); //Microsekunden
                std::cout << "Erfolgreich Entkommen\n";
                usleep(1000 *1000); //Mikrosekunden
                return showMainMenu_S;
                break;
                default:
                return Kampf_S;
                break;
            }
            }
            return Kampf_S;
        }
    public:
        int exec(){
            while(nextState_ != -1){
                switch(nextState_)
                {
                    case Kampf_S:
                        nextState_ = Kampf();
                        break;
                    case showKampf_S:
                        nextState_ = showKampf();
                        break;
                    case Besiegt_S:
                        nextState_ = Besiegt();
                        break;
                    case showComsumables_S:
                        nextState_ = showConsumables();
                        break;
                    case showCharacter_S:
                        nextState_ = showCharacter();
                        break;
                    case showShop_S:
                        nextState_ = showShop();
                        break;
                    case showMainMenu_S:
                        nextState_ = showMainMenu();
                        break;
                    case StartScreen_S:
                        nextState_ = StartScreen();
                        break;
                    default:
                    [[fallthrough]]; //Überprüfen
                    case exit_S:
                        return exit();
                        break;

                }
            }
            std::cout << "You shouldn't be here" << std::endl;
            return 1; // unreachable

        }
            StateEngine_T(const std::string& dbfile){
            if (!std::filesystem::exists(dbfile)) {
                // Erstellen Sie eine leere Datei, wenn sie nicht existiert
                std::ofstream(dbfile).close();
            }

            std::ifstream databasefile{dbfile};
            if (databasefile.peek() != std::ifstream::traits_type::eof()) {
                db = nlohmann::json::parse(databasefile);
                for (const auto& jsonHeld : db["Held"]) {
                    Helden held(
                        jsonHeld["Name"],
                        jsonHeld["Klasse"],
                        jsonHeld["Rasse"],
                        jsonHeld["Health"],
                        jsonHeld["MaxHealth"],
                        jsonHeld["EXP-Points"],
                        jsonHeld["Level"],
                        jsonHeld["AttackDamage"],
                        jsonHeld["MagicDamage"]
                    );
                    Held.push_back(held);
                }

            } else {
                // Initialisieren Sie die Datenbank mit einem leeren Objekt, falls die Datei leer ist
                db = nlohmann::json::object();
            }

            if (db.contains("coins")) {
                coins = db["coins"].get<int>();
            }
            if (db.contains("numbpotion")) {
                numbpotion = db["numbpotion"].get<int>();
            }

            // erster Zustand wird die Menüanzeige
            nextState_ = StartScreen_S;

        }


    };

};  

int main(int argc, char** argv){
    CLI::App thisApplication{"Final Slug Final Fanatsy", "Spiel"};
    
    std::string dbfilename;
    thisApplication.add_option("-f", dbfilename, "Path to Database-File");

    try{
        //thisApplication.parse(argc, argv);
        CLI11_PARSE(thisApplication, argc, argv);
    } catch(const CLI::ParseError& e){
        return thisApplication.exit(e);
    }

    // Zunächste wird die Statemachine im Speicher angelegt
	local::StateEngine_T states{dbfilename};
	// Im Anschluss daran wird die eigentliche Engine ausgeführt.
	return states.exec();

}

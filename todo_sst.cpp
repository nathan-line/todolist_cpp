#include <iostream> 
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

string get_date(){

    // Récupération de la date
    time_t actuel = time(0);
    tm *ltm = localtime(&actuel);
    string creation = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
    return creation;
}
     
class tache { 

    friend class todolist ;

    protected:

    int ID;
    string titre;
    string description;
    string creation;
    string cloture;
    int status; // 0: ouvert, 1: fermé, 2: en progression.
    float avancement;
    string priorite;
    string commentaires;



    public:

    tache(int X, string a, string b, string c, string d, int Y, int Z, string e, string f) : ID(X), titre(a), description(b), creation(c), cloture(d), status(Y),
    avancement(Z), priorite(e), commentaires(f) {}

    void afficher_tache() {
            cout << ID << " | " << titre << " | " << description << " | " << creation << " | " << cloture << " | " << get_status() << " | " << avancement << " %" << " | " <<
            priorite << " | " << commentaires << " | " << endl;
    }

    void fermer_tache() {
        status = 1;
         // obtention de la date de fermeture :
        cloture = get_date();
    }

    void changement_tache(string el, string NewValue){
        if (el == "--title")
            titre = NewValue;
        if (el == "--priority")
            priorite = NewValue;
        
        if (el == "--description")
            description == NewValue;
    }
    
    void ajout_commentaire(string a){
        commentaires = commentaires + " " + a; // concaténation
    }

    void changement_avancement(int a){
        if (0 <a < 100){
            avancement = a;
            status = 2;
        }
        else if (a = 0){
            avancement = a;
            status = 0;
        }
        else if (a = 100){
            avancement = a;
            status = 1;
        }
        else{
            cout << "La valeur indiquée n'est pas valide";
        }
    }

    string get_status(){
        if (status == 0)
            return "ouvert";
        if (status == 1)
            return "fermé";
        if (status == 2)
            return "en progression";
    }

    void write_task_csv_t(string path){
        ofstream todo1;
        todo1.open(path, ios::trunc);
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << "\n";
        todo1.close();
        }
    
    void write_task_csv_a(string path){
        ofstream todo1;
        todo1.open(path, ios::app);
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << "\n";
        todo1.close();
        }
};



class todolist{

    protected : 

    vector<tache> ensemble_des_taches;

    public:

    todolist (vector<tache> tasks) : ensemble_des_taches(tasks) {};
    /*todolist (const string& text) : {
     }*/

    void ajout_tache(tache T) {
        ensemble_des_taches.push_back(T);
    }

    int Nombre_taches(){
        return ensemble_des_taches.size();
    }

    void afficher_tout() {
        for (int i = 0; i < ensemble_des_taches.size(); i++)
        {
            tache T = ensemble_des_taches[i] ;
            cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
            T.priorite << " | " << T.commentaires << " | " << endl;
        }   
    }

    void afficher(string priority){
        for (int i = 0; i < ensemble_des_taches.size(); i++)
        {
            tache T = ensemble_des_taches[i] ;
            if (T.priorite == priority){
            cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
            T.priorite << " | " << T.commentaires << " | " << endl;
            }
            }
        }  

    void fermer(int id){
        ensemble_des_taches[id].fermer_tache();
    } 

    void write_todo_csv(string path){
        if (ensemble_des_taches.size() >0){
            // On écrit le premier en troncature, pour effacer le reste du document et le mettre dessus.
            // On écrit la suite en addition.
            ensemble_des_taches[0].write_task_csv_t(path);
            for (int i = 1; i<ensemble_des_taches.size(); i++){
                ensemble_des_taches[i].write_task_csv_a(path);
            }
        }
        else{
            // On nettoie la todolist : l'ouverture en mode troncature le fait.
            ofstream todo1;
            todo1.open(path, ios::trunc);
            todo1.close();
        }
    }

    tache * get_pointeur_tache(int i){
            return &(ensemble_des_taches[i]);
    }

    void remove(int id){
        ensemble_des_taches.erase(ensemble_des_taches.begin() + id) ;
        for (int i = id; i<ensemble_des_taches.size(); i++)
            ensemble_des_taches[i].ID --;
    }

    void clean(){
        ensemble_des_taches = {};
    }
};

int main(int argc, char* argv[]){

    /// I. On convertit le doc en objet de type todolist:
    
    // 1. Récupérations des lignes sous forme d'un vector<vector<string>>

    ifstream todo("ToDoList.csv");

    string line;
    vector<vector<string>> cont;

    while (getline(todo, line)){
        
        //split : 

        stringstream ss(line);
        vector<string> split_line;
        string token;

        while (getline(ss, token, ';')) {
            split_line.push_back(token);
            }
        cont.push_back(split_line);
        }

    // 2. Transcription en todolist

    vector<tache> taches = {};

    for (int i = 0; i<cont.size(); i++){
        tache T1 = tache(stoi(cont[i][0]), cont[i][1], cont[i][2], cont[i][3], cont[i][4], stoi(cont[i][5]), stof(cont[i][6]), cont[i][7], cont[i][8]);
        // stoi : convertit un string en int, stof: convertit un string en float
        taches.push_back(T1);
    }

    todolist Todo = todolist(taches);

    // II. Opérations sur la Todo-list

    vector<string> args(argv, argv + argc);
    
    string commande = args[1];
    
    if (commande == "list")
    {
        if ((argc >= 3) && (args[2] == "--priority"))
        {
            if (argc >= 4 )
            {
                Todo.afficher(args[3]);
            }
        }
        else{
            Todo.afficher_tout();
        }
    }


    if (commande == "create")
    {
        if ((argc = 8 ) && (args[2] == "--title") && (args[4] == "--description") && (args[6] == "--priority"))
        {
            int Id = cont.size();
            tache T = tache(Id, args[3], args[5], get_date(), " ", 0, 0.0, args[7], " ");
            Todo.ajout_tache(T);
        }
    }

    if (commande == "close")
    {
        if ((argc >= 4) && (args[2] == "--id")){ // Il faut aussi tester que args[3] est un entier
                int id = stoi(args[3]);
                Todo.fermer(id);
        }

    }
    
    if (commande == "display"){
        if ((argc>=4) && (args[2] == "--id")){
            int id = stoi(args[3]);
            tache * T = Todo.get_pointeur_tache(id);
            T->afficher_tache();
        }
    }

    if (commande == "modify")
    {
        if ((argc >= 6) && args[2] == "--id"){
            int id = stoi(args[3]);
            for (int i = 4; i<argc-1; i= i+2){
                    tache * p = Todo.get_pointeur_tache(id);
                    if ((args[i] == "--title") || (args[i] == "--description") || (args[i] == "--priority")){
                        p->changement_tache(args[i], args[i+1]);
                    }
                    if (args[i] == "--comment"){
                        p->ajout_commentaire(args[i+1]);
                    }

                }

            }
       
        }

    if (commande == "clear"){
        cout << "Vous êtes sur le point de supprimer toutes les tâches existantes. Continuer ? [Y/N]"<<endl;
        string a ;
        cin >> a;
        while ((a!="Y") && (a!="N")){
            cin>>a;
        }
        if (a=="Y"){
            Todo.clean();
            Todo.afficher_tout();
        }
    }

    if (commande == "remove")
        if ((argc>=4) && (args[2] == "--id")){
                int id = stoi(args[3]);
                Todo.remove(id);
            }
    
    // III. Mise à jour du csv :

    Todo.write_todo_csv("ToDoList.csv");

    return 0;
}

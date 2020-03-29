#include <iostream> 
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

string get_date(){
    // Fonction pour récupérer la date actuelle. Utilisée pour créer et clore des tâches.

    time_t actuel = time(0);
    tm *ltm = localtime(&actuel);
    string date = to_string(ltm->tm_mday) + "/" + to_string(1 + ltm->tm_mon) + "/" + to_string(1900 + ltm->tm_year);
    return date;
    }
     
class tache { 
    //Classe tâche, qui correspond à une tâche à effectuer de la todo-list.

    friend class todolist ; // la classe todolist est mise en friend, car elle a besoin d'avoir l'accès sur les membres de la classe tâche pour les afficher.

    protected:

    int ID;
    string titre;
    string description;
    string creation;
    string cloture;
    int status; // 0: ouvert, 1: fermé, 2: en progression.
    int avancement; // int entre 0 et 100.
    string priorite;
    string commentaires;

    
    public:

    tache(int X, string a, string b, string c, string d, int Y, int Z, string e, string f) : ID(X), titre(a), description(b), creation(c), cloture(d), status(Y),
    avancement(Z), priorite(e), commentaires(f) {}

    string get_status(){
        if (status == 0)
            return "ouvert";
        if (status == 1)
            return "fermé";
        if (status == 2)
            return "en progression";
    }

    void afficher_tache() {
            cout << ID << " | " << titre << " | " << description << " | " << creation << " | " << cloture << " | " << get_status() << " | " << avancement << " %" << " | " <<
            priorite << " | " << commentaires << " | " << endl;
    }

    void fermer_tache() {
        status = 1;
        cloture = get_date(); // date de fermeture
    }

    void changement_tache(string element, string NewValue){
        if (element == "--title")
            titre = NewValue;
        if (element == "--priority")
            priorite = NewValue;
        if (element == "--description")
            description == NewValue;
    }
    
    void ajout_commentaire(string a){
        if (commentaires == " ")
            commentaires = a;
        else{
        commentaires = commentaires + " " + a; // concaténation, pour garder les anciens commentaires.
        }
    }

    void changement_avancement(int a){
        // On met à jour le statut, en considérant qu'à 0% la tâche ouverte, qu'à 100% la tâche est finie donc fermée, et que sinon elle est en progression.
        if ((a>0) && (a<100)){
            avancement = a;
            status = 2;
        }
        else if (a == 0){
            avancement = a;
            status = 0;
        }
        else if (a == 100){
            avancement = a;
            status = 1;
        }
        else{
            cout << "Valeur d'avancement incorrecte" << endl;
        }
    }

    /* Les deux méthodes suivantes sont créées pour être utilisées dans la méthode write_todo_csv de la classe todolist.
    La première écrit une tâche sur un csv (à l'addresse path) en mode troncature, donc en écrasant les tâches écrites avant, la seconde en mode addition. */

    void write_task_csv_t(string path){
        ofstream todo1;
        todo1.open(path, ios::trunc); //mode troncature
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << "\n";
        todo1.close();
        }
    
    void write_task_csv_a(string path){
        ofstream todo1;
        todo1.open(path, ios::app); //mode ajoût
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << "\n";
        todo1.close();
        }
};



class todolist{
    // Une classe pour représenter la todolist, et notamment lui donner des méthodes.

    protected : 

    vector<tache> ensemble_des_taches;


    public:

    todolist (vector<tache> tasks) : ensemble_des_taches(tasks) {};
    /*todolist (const string& text) : {
    }*/

    void ajout_tache(tache T) {
        ensemble_des_taches.push_back(T);
    }

    void afficher_tout() {
        // Pour afficher toutes les tâches de la todolist
        for (int i = 0; i < ensemble_des_taches.size(); i++){
            tache T = ensemble_des_taches[i] ;
            cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
            T.priorite << " | " << T.commentaires << " | " << endl;
        }   
    }

    void afficher(string priority){
        // Pour afficher seulement les tâches de la todolist avec la priorité voulue.
        for (int i = 0; i < ensemble_des_taches.size(); i++){
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

    // Une fonction pour écrire la todolist sur un csv :

    void write_todo_csv(string path){
        if (ensemble_des_taches.size() >0){
            ensemble_des_taches[0].write_task_csv_t(path); // On écrit la première tâche en troncature, pour effacer document d'avant.
            for (int i = 1; i < ensemble_des_taches.size(); i++){
                ensemble_des_taches[i].write_task_csv_a(path);
            }
        }
        else{
            //Si l'ensemble des tâches est vide: on nettoie la todolist. L'ouverture en mode troncature le fait.
            ofstream todo1;
            todo1.open(path, ios::trunc);
            todo1.close();
        }
    }

    tache * get_pointeur_tache(int i){
        // Pour récupérer une tâche spécifique, sans avoir uniquement une copie.
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

    // I. On convertit le document csv en objet de type todolist:
    
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
        tache T1 = tache(stoi(cont[i][0]), cont[i][1], cont[i][2], cont[i][3], cont[i][4], stoi(cont[i][5]), stoi(cont[i][6]), cont[i][7], cont[i][8]);
        // stoi : convertit un string en int
        taches.push_back(T1);
    }

    todolist Todo = todolist(taches);

    // II. Opérations sur la Todo-list

    vector<string> args(argv, argv + argc); // Pour avoir des strings comme arguments d'entrée. On utilisera donc args.
    
    string commande = args[1]; // récupération de la commande, puis switch sur sa valeur.
    

    if (commande == "list"){
        // liste les différentes tâches de la todolist.
        if (argc == 2){
            Todo.afficher_tout();
        }
        else if ((argc== 4) && (args[2] == "--priority")){
            Todo.afficher(args[3]);
        }

        else{
            cout << "Commande incorrecte" << endl;
        }
    }


    if (commande == "create"){ 
        // créé une nouvelle tâche à la fin de la todolist.

        if ((argc == 6) && (args[2] == "--title") && (args[4] == "--description")){
            // Cas sans préciser la priorité : elle est mise automatiquement sur 'normale'
            int Id = cont.size();
            tache T = tache(Id, args[3], args[5], get_date(), " ", 0, 0.0, "normale", " ");
            Todo.ajout_tache(T);
        }
        else if ((argc == 8 ) && (args[2] == "--title") && (args[4] == "--description") && (args[6] == "--priority")){
            // Cas avec priorité
            int Id = cont.size();
            tache T = tache(Id, args[3], args[5], get_date(), " ", 0, 0.0, args[7], " ");
            Todo.ajout_tache(T);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "close"){
        // Fait passer une tâche au statut fermé.
        if ((argc == 4) && (args[2] == "--id")){ // Il faut aussi tester que args[3] est un entier
                int id = stoi(args[3]);
                Todo.fermer(id);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }
    
    if (commande == "display"){
        // Affiche une tâche
        if ((argc==4) && (args[2] == "--id")){
            int id = stoi(args[3]);
            tache * T = Todo.get_pointeur_tache(id);
            T->afficher_tache();
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "modify"){
        // Modifie une tâche de la todolist.
        if ((argc >= 6) && args[2] == "--id"){
            int id = stoi(args[3]);

            for (int i = 4; i<argc-1; i= i+2){
                tache * p = Todo.get_pointeur_tache(id);
                if ((args[i] == "--title") || (args[i] == "--description") || (args[i] == "--priority")){
                    p->changement_tache(args[i], args[i+1]);
                }
                else{
                    cout << "Commande incorrecte" << endl;
                    break;
                }

            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "progress"){
        // Modifie l'avancement d'une tâche de la todolist, ainsi que son statut si besoin. 
        if ((argc>=6) && (args[2] == "--id") && (args[4] == "--percent")){
            int id = stoi(args[3]);
            int av = stoi(args[5]);

            tache * p = Todo.get_pointeur_tache(id);
            p->changement_avancement(av);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "comment"){
        // Ajoute un commentaire sur une tâche de la todolist.
        if ((argc == 6) && (args[2] == "--id") && (args[4] == "--text")){
            int id = stoi(args[3]);
            tache * p = Todo.get_pointeur_tache(id);

            p->ajout_commentaire(args[5]);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "remove"){
        // Retire une tâche de la todolist
        if ((argc==4) && (args[2] == "--id")){
                int id = stoi(args[3]);
                Todo.remove(id);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "clear"){
        // Nettoie la todolist, ie supprime toutes les tâches.
        if (argc==2){
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
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    // III. Mise à jour du csv :

    Todo.write_todo_csv("ToDoList.csv");

    return 0;
}
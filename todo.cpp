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
    int status; // 0: ouverte, 1: fermée, 2: en progression.
    int avancement; // int entre 0 et 100.
    string priorite;
    string commentaires;
    int ID_mere; // Identifiant de la tâche mère, pour les sous-tâches. Mis à -1 si la tâche est une tâche principale.

    
    public:

    tache(int X, string a, string b, string c, string d, int Y, int Z, string e, string f, int T) : ID(X), titre(a), description(b), creation(c), cloture(d), status(Y),
    avancement(Z), priorite(e), commentaires(f), ID_mere(T) {}

    string get_status(){
        if (status == 0)
            return "ouvert";
        if (status == 1)
            return "fermée";
        if (status == 2)
            return "en progression";
    }

    void fermer_tache() {
        status = 1;
        cloture = get_date(); // date de fermeture
    }
    

    /* Les deux méthodes suivantes sont créées pour être utilisées dans la méthode write_todo_csv de la classe todolist.
    La première écrit une tâche sur un csv (à l'addresse path) en mode troncature, donc en écrasant les tâches écrites avant, la seconde en mode ajoût. */

    void write_task_csv_t(string path){
        ofstream todo1;
        todo1.open(path, ios::trunc); //mode troncature
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << ";" << ID_mere << "\n";
        todo1.close();
        }
    
    void write_task_csv_a(string path){
        ofstream todo1;
        todo1.open(path, ios::app); //mode ajoût
        todo1 << to_string(ID) << ";" << titre << ";" << description << ";" << creation << ";" << cloture << ";" << to_string(status)
        << ";" << to_string(avancement) << ";" << priorite << ";" << commentaires << ";" << ID_mere << "\n";
        todo1.close();
        }
};



class todolist{
    // Une classe pour représenter la todolist, et notamment lui donner des méthodes.

    protected : 

    vector<tache> ensemble_des_taches;


    public:

    todolist (vector<tache> tasks) : ensemble_des_taches(tasks) {}

    void ajout_tache(string title, string desc, string priority) {
        // Ajout de tâche principale à la todolist

        int N_taches = ensemble_des_taches.size();
        tache T = tache(N_taches, title, desc, get_date(), " ", 0, 0, priority, " ", -1);
        ensemble_des_taches.push_back(T);
    }

    void ajout_s_tache(string title, string desc, int Id_m){
        // Ajout de sous-tâche à la todo-list. C'est une tâche classique, avec un ID_mere != -1.

        int i = Id_m + 1;
        // On récupère la position à laquelle il faut mettre la nouvelle tâche :
        while (ensemble_des_taches[i].ID_mere == Id_m){
            i++;
        }
        tache T = tache(i, title, desc, get_date(), " ", 0, 0, ensemble_des_taches[Id_m].priorite," ", Id_m); // La priorité de la sous_tache est mise à celle de la tâche principale.
        ensemble_des_taches.insert(ensemble_des_taches.begin() + i, T); // Pour que l'indice reste la position dans le tableau.

        for (int j = i+1; j<ensemble_des_taches.size(); j++){
            // Mise à jour de l'indice
            ensemble_des_taches[j].ID++;

            //Mise à jour de l'Id_m pour les sous-tâches. Pas de problèmes avec les autres sous-tâches de la même tâche, puisque la nouvelle sous-tâche est mise à la fin.
            if (ensemble_des_taches[j].ID_mere != -1){
                ensemble_des_taches[j].ID_mere++;
            }
        }       
    }

    vector<vector<int>> get_sous_taches(){
        // Parcours de la todo list pour identifier les sous-tâche : get_sous_taches[i] est un tableau d'entier correspondant aux sous-tâches de la tâche i.
        // Les tableaux obtenus sont ordonnés.
        int N_taches = ensemble_des_taches.size();
        vector<int> empt = {};
        vector<vector<int>> Tab(N_taches, empt);

        for (int i = 0; i < N_taches; i++){ 
            tache T1 = ensemble_des_taches[i];
            if (T1.ID_mere !=-1){
                Tab[T1.ID_mere].push_back(i);
            }
        }
        return Tab;
    }

    bool not_soustache(int id){
        // Test si la tâche d'indice id est une tâche principale.
        return (ensemble_des_taches[id].ID_mere == -1);
    }


    void afficher_tache(int id) {
        tache T = ensemble_des_taches[id];

        cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
        T.priorite << " | " << T.commentaires << " | " << endl;
    }

    void afficher_tout() {
        // Pour afficher toutes les tâches de la todolist, avec un affichage spécial pour les sous-tâches d'une tâche.

        vector<vector<int>> Tab = get_sous_taches();

        for (int i = 0; i < ensemble_des_taches.size(); i++){
            tache T = ensemble_des_taches[i];

            if (T.ID_mere == -1){

                //Tache principale :
                cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
                T.priorite << " | " << T.commentaires << " | " << endl;

                //Sous_tâches.
                for (int j: Tab[i]){
                    tache sT = ensemble_des_taches[j];
                    cout << "   " << sT.ID << " | " << sT.titre << " | " << sT.description << " | " << sT.creation << " | " << sT.cloture << " | " << sT.get_status() << " | " << sT.avancement << " %" << " | " <<
                    sT.priorite << " | " << sT.commentaires << " | " << endl;  
                }
            }
        }   
    }

    void afficher(string priority){
        // Pour afficher seulement les tâches de la todolist avec la priorité voulue. Meme fonction qu'avant, avec un if en plus.

        vector<vector<int>> Tab = get_sous_taches();
        // Affichage des tâches principales, avec leurs sous-tâches.    
        for (int i = 0; i < ensemble_des_taches.size(); i++){
            tache T = ensemble_des_taches[i];

            if ((T.ID_mere == -1) && (T.priorite == priority)){

                //Tache principale :
                cout << T.ID << " | " << T.titre << " | " << T.description << " | " << T.creation << " | " << T.cloture << " | " << T.get_status() << " | " << T.avancement << " %" << " | " <<
                T.priorite << " | " << T.commentaires << " | " << endl;

                // Sous-tâches (même priorité que la tâche principale)
                for (int j: Tab[i]){
                    tache sT = ensemble_des_taches[j];
                    cout << "   " << sT.ID << " | " << sT.titre << " | " << sT.description << " | " << sT.creation << " | " << sT.cloture << " | " << sT.get_status() << " | " << sT.avancement << " %" << " | " <<
                    sT.priorite << " | " << sT.commentaires << " | " << endl;  
                }
            }
        }   
    }


    void fermer(int id){
        // 1. Fermeture de la tâche principale.
        ensemble_des_taches[id].fermer_tache();

        // 2. Fermeture de ses sous-tâches. :
        if (ensemble_des_taches[id].ID_mere == -1){
            vector<int> indices_st = get_sous_taches()[id];
            for (int i: indices_st){
                ensemble_des_taches[i].fermer_tache();
            }
        }
    } 


    void changement_avancement(int id, int av){

        // On met à jour le statut, en considérant qu'à 0% la tâche est ouverte, qu'à 100% la tâche est finie donc fermée, et que sinon elle est en progression.

        tache * p = &(ensemble_des_taches[id]);

        if ((av>0) && (av<100)){
            p->avancement = av;
            p->status = 2;
        }
        else if (av == 0){
            p->avancement = av;
            p->status = 0;
        }
        else if (av == 100){
            p->avancement = av;
            p->status = 1;
        }
    }

    void ajout_commentaire(int id, string com){
        tache * p = &(ensemble_des_taches[id]);

        if (p->commentaires == " "){
            p->commentaires = com;
        }
        else{
            p->commentaires = p->commentaires + " " + com; // concaténation, pour garder les anciens commentaires.
        }
    }

    void changement_tache(int id, string element, string NewValue){
        tache * p = &(ensemble_des_taches[id]);

        if (element == "--title")
            p->titre = NewValue;
        else if (element == "--description")
            p->description = NewValue;
    }

    void changement_prio(int id, string NewValue){
        tache * p = &(ensemble_des_taches[id]);

        if (not_soustache(id)){
            // Si tâche principale, on change la tâche et on met à jour ses sous-tâches.
            p->priorite = NewValue;

            vector<int> indices_st = get_sous_taches()[id];
            for (int j : indices_st){
                tache * p1 = &(ensemble_des_taches[j]);
                p1->priorite = NewValue;
            }
        }
        else{
            cout << "Modification impossible de la priorité d'une sous-tâche" << endl;
        }
    }

    void remove(int id){
        if (not_soustache(id)){
            vector<int> indices_st = get_sous_taches()[id];

            for (int j = indices_st.size()-1; j>=0; j--){ 
                //boucle à l'envers : le but est de supprimer d'abbord les tâches les plus loins dans la liste pour ne pas avoir de problème d'indice.
                ensemble_des_taches.erase(ensemble_des_taches.begin() + indices_st[j]);

            }

        ensemble_des_taches.erase(ensemble_des_taches.begin() + id) ; 
            for (int i = id; i<ensemble_des_taches.size(); i++){

                // Mise à jour des identifiants des autres tâches.
                ensemble_des_taches[i].ID -= indices_st.size() + 1;

                // Mise à jour des Id_m
                    if (ensemble_des_taches[i].ID_mere != -1){
                        ensemble_des_taches[i].ID_mere-= indices_st.size() + 1;
                    }
            }
        }
        else{
            int buf = ensemble_des_taches[id].ID_mere;
            ensemble_des_taches.erase(ensemble_des_taches.begin() + id) ;
            for (int i = id; i<ensemble_des_taches.size(); i++){

                // Mise à jour des identifiants des autres tâches.
                ensemble_des_taches[i].ID --;

                // Mise à jour des Id_m : Il faut faire descendre d'un l'Id_m pour les sous tâches telles que leur tâche est descendue d'1, donc les sous tâches du
                // dessus sans compter celle issue de la même tâche. 
                if ((ensemble_des_taches[i].ID_mere != -1) && (ensemble_des_taches[i].ID_mere != buf)) {
                    ensemble_des_taches[i].ID_mere--;
                }
            }
        }
    }


    void clean(){
        ensemble_des_taches = {};
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
        tache T1 = tache(stoi(cont[i][0]), cont[i][1], cont[i][2], cont[i][3], cont[i][4], stoi(cont[i][5]), stoi(cont[i][6]), cont[i][7], cont[i][8], stoi(cont[i][9]));
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
            Todo.ajout_tache(args[3], args[5], "normale");
        }
        else if ((argc == 8 ) && (args[2] == "--title") && (args[4] == "--description") && (args[6] == "--priority")){
            // Cas avec priorité
            Todo.ajout_tache(args[3], args[5], args[7]);
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "add"){
        // Ajoute une sous-tâche à une tâche. C'est à dire créée une tâche de même priorité, et ayant pour ID_mere l'ID de la tâche.

        if ((argc == 8) && (args[2] == "--id") && (args[4] == "--title") && (args[6] == "--description")){
            int Id_m = stoi(args[3]);

            if (Todo.not_soustache(Id_m)){
                Todo.ajout_s_tache(args[5], args[7], Id_m);
            }
            else{
                cout << "Une sous-tâche ne peut pas avoir de sous-tâche." << endl;
            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "close"){
        // Fait passer une tâche au statut fermé.
        if ((argc == 4) && (args[2] == "--id")){ // Il faut aussi tester que args[3] est un entier
            int id = stoi(args[3]);

            if ((id >=0) && (id < cont.size())){
                Todo.fermer(id);
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }
    
    if (commande == "display"){
        // Affiche une tâche
        if ((argc==4) && (args[2] == "--id")){
            int id = stoi(args[3]);
            if ((id >=0) && (id < cont.size())){
                Todo.afficher_tache(id);
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "modify"){
        // Modifie une tâche de la todolist.
        if ((argc >= 6) && args[2] == "--id"){
            int id = stoi(args[3]);

            if ((id >=0) && (id < cont.size())){
                for (int i = 4; i<argc-1; i= i+2){
                    
                    if ((args[i] == "--title") || (args[i] == "--description")){
                        Todo.changement_tache(id, args[i], args[i+1]);
                    }
                    // cas particulier pour la priorité, qui doit rester la même pour une tâche et sa sous-tâche.
                    else if (args[i] == "--priority"){
                        Todo.changement_prio(id, args[i+1]);
                    }
                    else{
                        cout << "Commande incorrecte" << endl;
                        break;
                    }
                }
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
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

            if ((id >=0) && (id < cont.size())){
                int av = stoi(args[5]);

                if ((av >=0) && (av <=100)){
                    Todo.changement_avancement(id, av);
                }
                else{
                    cout << "Valeur d'avancement incorrecte" << endl;
                } 
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "comment"){
        // Ajoute un commentaire sur une tâche de la todolist.
        if ((argc == 6) && (args[2] == "--id") && (args[4] == "--text")){
            int id = stoi(args[3]);

            if ((id >=0) && (id < cont.size())){
                Todo.ajout_commentaire(id, args[5]);
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
            }
        }
        else{
            cout << "Commande incorrecte" << endl;
        }
    }

    if (commande == "remove"){
        // Retire une tâche de la todolist
        if ((argc==4) && (args[2] == "--id")){
            int id = stoi(args[3]);
            if ((id >=0) && (id < cont.size())){
                Todo.remove(id);
            }
            else{
                cout << "Cette tâche n'existe pas" << endl;
            }
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
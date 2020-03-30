#include "todo.h"


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
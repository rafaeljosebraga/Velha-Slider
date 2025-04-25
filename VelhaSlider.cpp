#include <iostream>
#include <vector>
#include <cctype>
#include <limits>

using namespace std;

typedef vector<vector<int>> mov;

//busca por largura para encontrar a melhor jogada para o segundo jogador(Agente IA)
mov acoes(char Peca,vector<vector<char>> &map){
    // Implementação da busca por largura
    // listar todos as jogadas comuns
    mov movimentos;
    for(int i = 0;i < 3;i++){
        for(int j = 0;j < 3;j++){
            if(map[i][j] == '_'){
            movimentos.push_back({i,j});
            }
        }
    }
    return movimentos;
}

//mostra o mapa
void mostra(vector<vector<char>> &map){
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++){
            cout << map[x][y] << ' ';
        }
        cout << endl;
    }
}

//função que checa a vitória
int win_check(vector<vector<char>> &map){
    //checa linhas e colunas
    for (int i = 0; i < 3; ++i) {
        if (map[i][0] != '_' && map[i][0] == map[i][1] && map[i][1] == map[i][2]) return 1;
        if (map[0][i] != '_' && map[0][i] == map[1][i] && map[1][i] == map[2][i]) return 1;
    }

    // Checa diagonais
    if (map[0][0] != '_' && map[0][0] == map[1][1] && map[1][1] == map[2][2]) return 1;
    if (map[0][2] != '_' && map[0][2] == map[1][1] && map[1][1] == map[2][0]) return 1;

    int counter = 0;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++){
            if(map[x][y] != '_') {counter++;}
        }
    }
    if (counter == 9)return 0;
    return -1;
}

//função que coloca, recebe o mapa, coordenadas X e Y, e o char a ser colocado
void place(vector<vector<char>> &map,int x,int y,char C){
    map[x][y] = C;
}

//função que desliza rece o mapa, se vai ser coluna ou linha e index de qual vai ser
void slide(vector<vector<char>> &map,char escolha,int index,char dir){
    char aux;
    //escolha de linhas
    if(escolha == 'l'){
        vector<char>& linha = map[index];//pega referencia do vetor da linha direto
        if(dir == 'd'){//desliza pra direita
            aux = linha[2];
            linha[2] = linha[1];
            linha[1] = linha[0];
            linha[0] = aux;
        }
        else{//desliza pra esquerda
            aux = linha[0];
            linha[0] = linha[1];
            linha[1] = linha[2];
            linha[2] = aux;
        }

    } else{ //escolha de colunas
        if(dir == 'b'){//desliza pra baixo
            aux = map[2][index];
            map[2][index] = map[1][index];
            map[1][index] = map[0][index];
            map[0][index] = aux;
        }else{//desliza pra cima
            aux = map[0][index];
            map[0][index] = map[1][index];
            map[1][index] = map[2][index];
            map[2][index] = aux;
        }
    }
}

//Calcula a melhor jogada usando minimax
//deve retornar uma cordenada
int minimax(vector<vector<char>> &map, bool isMaximizing, bool jogadorD){
    //A ia precisa prever também a ação de deslizar pelo jogador quando ele pode deslizar
    int result = win_check(map);
    if (result == 1) return isMaximizing ? -1 : 1; // vitória
    if (result == 0) return 0; // empate

    int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (map[i][j] == '_'){
                map[i][j] = isMaximizing ? 'O' : 'X';
                int score = minimax(map, !isMaximizing,jogadorD);
                map[i][j] = '_';

                if (isMaximizing)
                    bestScore = max(score, bestScore);
                else
                    bestScore = min(score, bestScore);
            }
        }
    }
    //achar o best score verificando tambem se o jogador pode deslizar
    return bestScore;
}

vector<int> Agente_joga(vector<vector<char>> &map,bool maquinaD,bool jogadorD){
    int bestScore = numeric_limits<int>::min();
    vector<int> bestMove = {-1, -1};

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (map[i][j] == '_'){
                map[i][j] = 'O';
                int score = minimax(map, false,jogadorD);
                map[i][j] = '_';

                if (score > bestScore){
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }

    //verifica se pode delisar
    if(!maquinaD){
    //testa Deslizar
        for(int i = 0; i < 2; i++){
            if(i == 0){
                //testa delisar linha
                for(int counter = 0; counter < 3; counter++){
                    //verificar as duas possibilidades para cada um
                    // if()
                    // if()
                }
            }
            else{
                //testa delisar coluna
                for(int counter = 0; counter < 3; counter++){
                    //verificar as duas possibilidades para cada um
                    //if()
                    // if()
                    //deslizar cada coluna
                }
            }
        }
    }
    return bestMove;
}

int main(){

   //cria o mapa
   vector<vector<char>> map = {
        {'_', '_', '_'},
        {'_', '_', '_'},
        {'_', '_', '_'}
   };

   //variaveis de Input do jogador
   int seletX = 0,seletY = 0;
   char Pescolha;
   int index;
   char dir;
   //Variaveis do jogo
   bool jogadorD;
   bool maquinaD;
   jogadorD = false;
   maquinaD = false;

   //começa o jogo mostrando o mapa
   mostra(map);

    //loop de jogadas
    while(true){
        cout << "Digite 'D' se quiser deslizar ou 'C' se quiser colocar :" << endl;
        cin >> Pescolha;
        Pescolha = tolower(Pescolha); // normaliza a entrada para minúscula

        if(Pescolha != 'd' && Pescolha != 'c'){
            cout << "Escolha inválida! Tente novamente." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpa o buffer de entrada
            continue; // volta para o começo do loop
        }

        if(Pescolha == 'c'){
            cout << "escolha onde quer colocar com coordenadas:" << endl;
            //input do jogador sobre onde vai colocar impedindo que ele coloque letras
            if (!(cin >> seletX >> seletY)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida: use números.\n";
                continue;
            }
            cout << endl;

            // Verifica se as coordenadas estão dentro do intervalo válido
            if(seletX < 1 || seletX > 3 || seletY < 1 || seletY > 3) {
                cout << "Coordenadas inválidas! Tente novamente." << endl;
                continue; // volta para o começo do loop
            }
            
            seletX --;
            seletY --;

            if (map[seletX][seletY] != '_') {
                cout << "Posição já ocupada! Tente outra." << endl;
                continue; // volta para o começo do loop
            }

            //coloca a peça no mapa
            if(jogadorD == true)jogadorD=false;
            place(map,seletX,seletY,'X');
        } else{
            if(jogadorD == true){
                cout << "Você não pode deslizar duas vezes seguidas\n";
                continue;
            }

            cout << "Deslizar linha ou uma coluna ('L' ou 'C')?" << endl;
            cin >> Pescolha;
            Pescolha = tolower(Pescolha); // normaliza a entrada para minúscula

            if(Pescolha != 'l' && Pescolha != 'c'){
                cout << "Escolha inválida! Digite ('L' ou 'C')." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if(Pescolha == 'l'){
                cout << "qual linha? (1, 2 ou 3): ";
                cin >> index;
                if(index < 1 || index > 3){
                    cout << "Índice inválido. Tente novamente." << endl;
                    continue;
                }
                cout << "Esquerda ou direita? ('E' ou 'D'): ";
                cin >> dir;
                dir = tolower(dir); // normaliza a entrada para minúscula
                if(dir != 'e' && dir != 'd'){
                    cout << "Direção inválida! Use ('E' ou 'D')." << endl;
                    continue;
                }
            }
            else { // Pescolha == 'c'
                cout << "qual coluna? (1, 2 ou 3): ";
                cin >> index;
                if(index < 1 || index > 3){
                    cout << "Índice inválido. Tente novamente." << endl;
                    continue;
                }
                cout << "Cima ou baixo? ('C' ou 'B'): ";
                cin >> dir;
                dir = tolower(dir); // normaliza a entrada para minúscula
                if(dir != 'c' && dir != 'b'){
                    cout << "Direção inválida! Use ('C' ou 'B')." << endl;
                    continue;
                }
            }
            //desliza
            index--;
            slide(map, Pescolha, index, dir);
            jogadorD = true;
        }

        //checa a vitória
        if(win_check(map)== 1){

            mostra(map);
            cout << "VOCE GANHOOOO !!!" << endl;

            break;
        }

        //Antes disso é player Depois é a IA
        vector<int> jogada = Agente_joga(map,maquinaD,jogadorD);
        if(jogada[0] == -1 || jogada[1] == -1) {
            cout << "Empate!" << endl;
            break;
        }
        //mostra a jogada da IA
        cout << "jogada da IA: " << jogada[1] + 1 << " " << jogada[0] + 1  << endl;

        //realizar jogada
        place(map,jogada[0],jogada[1],'O');

        //checa a vitoria, se alguem ganhar mostra o mapa termina a anuncia o vencedor
        if(win_check(map)== 1){
            mostra(map);
            cout << "ROBO GANHOOOO !!!" << endl;
            break;
        }
        mostra(map);//mostra o sempre depois da jogada da IA a não ser que o jogador tenha vencido
    }
   return 0;
}

#include <iostream>
#include <vector>
#include <cctype>
#include <limits>


using namespace std;


typedef vector<vector<int>> mov;


// busca por largura para encontrar a melhor jogada para o segundo jogador (Agente IA)
mov acoes(char Peca, vector<vector<char>> &map) {
    mov movimentos;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == '_') {
                movimentos.push_back({i, j});
            }
        }
    }
    return movimentos;
}


// mostra o mapa
void mostra(vector<vector<char>> &map) {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            cout << map[x][y] << ' ';
        }
        cout << endl;
    }
}


// função que checa a vitória
typedef vector<vector<char>> Board;
int win_check(Board &map) {
    // checa linhas e colunas
    for (int i = 0; i < 3; ++i) {
        if (map[i][0] != '_' && map[i][0] == map[i][1] && map[i][1] == map[i][2]) return 1;
        if (map[0][i] != '_' && map[0][i] == map[1][i] && map[1][i] == map[2][i]) return 1;
    }
    // checa diagonais
    if (map[0][0] != '_' && map[0][0] == map[1][1] && map[1][1] == map[2][2]) return 1;
    if (map[0][2] != '_' && map[0][2] == map[1][1] && map[1][1] == map[2][0]) return 1;


    int counter = 0;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (map[x][y] != '_') counter++;
        }
    }
    if (counter == 9) return 0; // empate
    return -1;  // jogo continua
}


// função que coloca peça
void place(Board &map, int x, int y, char C) {
    map[x][y] = C;
}


// função que desliza (linha ou coluna)
void slide(Board &map, char escolha, int index, char dir) {
    char aux;
    if (escolha == 'l') {
        vector<char> &linha = map[index];
        if (dir == 'd') {
            aux = linha[2]; linha[2] = linha[1]; linha[1] = linha[0]; linha[0] = aux;
        } else {
            aux = linha[0]; linha[0] = linha[1]; linha[1] = linha[2]; linha[2] = aux;
        }
    } else {
        if (dir == 'b') {
            aux = map[2][index]; map[2][index] = map[1][index]; map[1][index] = map[0][index]; map[0][index] = aux;
        } else {
            aux = map[0][index]; map[0][index] = map[1][index]; map[1][index] = map[2][index]; map[2][index] = aux;
        }
    }
}


// minimax (avaliação recursiva)
int minimax(Board &map, bool isMaximizing, bool jogadorD) {
    int result = win_check(map);
    if (result == 1) return isMaximizing ? -1 : 1;
    if (result == 0) return 0;


    int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == '_') {
                map[i][j] = isMaximizing ? 'O' : 'X';
                int score = minimax(map, !isMaximizing, jogadorD);
                map[i][j] = '_';
                if (isMaximizing)
                    bestScore = max(score, bestScore);
                else
                    bestScore = min(score, bestScore);
            }
        }
    }
    return bestScore;
}


// Calcula a melhor jogada para IA, incluindo deslizes
vector<int> Agente_joga(Board &map, bool maquinaD, bool jogadorD) {
    int bestScore = numeric_limits<int>::min();
    vector<int> bestMove = {-1, -1}; // marcação padrão [i, j]


    // 1) Testa marcações
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == '_') {
                map[i][j] = 'O';
                int score = minimax(map, false, jogadorD);
                map[i][j] = '_';
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }


    // 2) Testa deslizes (se possível)
    if (!maquinaD) {
        // deslizes de linha
        for (int linha = 0; linha < 3; linha++) {
            for (int dir_code = 0; dir_code < 2; dir_code++) {
                char dir = (dir_code == 0 ? 'e' : 'd');
                slide(map, 'l', linha, dir);
                int score = minimax(map, false, jogadorD);
                slide(map, 'l', linha, (dir == 'e' ? 'd' : 'e')); // desfaz
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {0, linha, dir_code}; // tipo=0 (linha)
                }
            }
        }
        // deslizes de coluna
        for (int col = 0; col < 3; col++) {
            for (int dir_code = 0; dir_code < 2; dir_code++) {
                char dir = (dir_code == 0 ? 'c' : 'b');
                slide(map, 'c', col, dir);
                int score = minimax(map, false, jogadorD);
                slide(map, 'c', col, (dir == 'c' ? 'b' : 'c')); // desfaz
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {1, col, dir_code}; // tipo=1 (coluna)
                }
            }
        }
    }
    return bestMove;
}


int main() {
    // inicializa o tabuleiro vazio
    vector<vector<char>> map = {
        {'_', '_', '_'},
        {'_', '_', '_'},
        {'_', '_', '_'}
    };


    int seletX = 0, seletY = 0;
    char Pescolha;
    int index;
    char dir;
    bool jogadorD = false;
    bool maquinaD = false;


    mostra(map);
    while (true) {
        cout << "Digite 'D' se quiser deslizar ou 'C' se quiser colocar :" << endl;
        cin >> Pescolha;
        Pescolha = tolower(Pescolha);
        if (Pescolha != 'd' && Pescolha != 'c') {
            cout << "Escolha inválida! Tente novamente." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (Pescolha == 'c') {
            cout << "Escolha onde quer colocar com coordenadas:" << endl;
            if (!(cin >> seletX >> seletY)) {
                cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Entrada inválida: use números." << endl;
                continue;
            }
            if (seletX < 1 || seletX > 3 || seletY < 1 || seletY > 3) {
                cout << "Coordenadas inválidas! Tente novamente." << endl;
                continue;
            }
            seletX--; seletY--;
            if (map[seletX][seletY] != '_') {
                cout << "Posição já ocupada! Tente outra." << endl;
                continue;
            }
            place(map, seletX, seletY, 'X');
            jogadorD = false;
        } else {
            if (jogadorD) {
                cout << "Você não pode deslizar duas vezes seguidas" << endl;
                continue;
            }
            cout << "Deslizar linha ou coluna ('L' ou 'C')?" << endl;
            cin >> Pescolha;
            Pescolha = tolower(Pescolha);
            if (Pescolha != 'l' && Pescolha != 'c') {
                cout << "Escolha inválida!" << endl;
                continue;
            }
            if (Pescolha == 'l') {
                cout << "Qual linha? (1, 2 ou 3): "; cin >> index;
                if (index < 1 || index > 3) { cout << "Índice inválido." << endl; continue; }
                cout << "Esquerda ou direita? ('E' ou 'D'): "; cin >> dir; dir = tolower(dir);
                if (dir != 'e' && dir != 'd') { cout << "Direção inválida!" << endl; continue; }
            } else {
                cout << "Qual coluna? (1, 2 ou 3): "; cin >> index;
                if (index < 1 || index > 3) { cout << "Índice inválido." << endl; continue; }
                cout << "Cima ou baixo? ('C' ou 'B'): "; cin >> dir; dir = tolower(dir);
                if (dir != 'c' && dir != 'b') { cout << "Direção inválida!" << endl; continue; }
            }
            index--;
            slide(map, Pescolha, index, dir);
            jogadorD = true;
        }
        // checa vitória do jogador
        if (win_check(map) == 1) {
            mostra(map);
            cout << "VOCÊ GANHOU!!!" << endl;
            break;
        }
        // vez da IA
        vector<int> jogada = Agente_joga(map, maquinaD, jogadorD);
        if (jogada.size() == 2) {
            // jogada normal
            place(map, jogada[0], jogada[1], 'O');
            maquinaD = false;
            cout << "Jogada da IA: " << jogada[1] + 1 << " " << jogada[0] + 1 << endl;
        } else if (jogada.size() == 3) {
            // deslize da IA
            char escolha = (jogada[0] == 0 ? 'l' : 'c');
            int idx = jogada[1];
            char idir = (jogada[2] == 0 ? (escolha == 'l' ? 'e' : 'c') : (escolha == 'l' ? 'd' : 'b'));
            slide(map, escolha, idx, idir);
            maquinaD = true;
            cout << "IA desliza "
                 << (escolha=='l'?"linha ":"coluna ")
                 << idx+1 << " para "
                 << (idir=='e'||idir=='c' ? (idir=='e'?"esquerda":"cima") : (idir=='d'?"direita":"baixo"))
                 << endl;
        } else {
            cout << "Empate!" << endl;
            break;
        }
        // checa vitória da IA
        if (win_check(map) == 1) {
            mostra(map);
            cout << "ROBO GANHOU!!!" << endl;
            break;
        }
        mostra(map);
    }
    return 0;
}

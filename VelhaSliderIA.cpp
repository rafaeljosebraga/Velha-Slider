#include <iostream>
#include <vector>
#include <cctype>
#include <limits>

using namespace std;

typedef vector<vector<int>> mov;
typedef vector<vector<char>> Board;

enum MoveType {
   PLACE,  // Colocar peça
   SLIDE   // Deslizar linha/coluna
};

// Estrutura que representa um movimento
struct Move {
    MoveType type;  // Tipo de movimento
    int x;          // Coordenada x (para PLACE) ou 0 para linha, 1 para coluna (para SLIDE)
    int y;          // Coordenada y (para PLACE) ou índice da linha/coluna (para SLIDE)
    char dir;       // Direção do deslize: 'e'/'d' para linhas, 'c'/'b' para colunas
    // Construtor padrão
    Move() : type(PLACE), x(0), y(0), dir(0) {}
    // Construtor para movimento de colocação
    Move(int x, int y) : type(PLACE), x(x), y(y), dir(0) {}
    // Construtor para movimento de deslize
    Move(int escolha, int idx, char direction) : type(SLIDE), x(escolha), y(idx), dir(direction) {}
};

// Inverte a direção de um deslize
char inverseDir(char dir) {
    if (dir == 'e') return 'd';
    if (dir == 'd') return 'e';
    if (dir == 'c') return 'b';
    if (dir == 'b') return 'c';
    return 0;
}

// Gera todos os movimentos possíveis
vector<Move> acoes(Board &map, bool podeSlide) {
    vector<Move> movimentos;

    // Movimentos de colocar peça
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (map[i][j] == '_') {
                movimentos.push_back(Move(i, j));
            }
        }
    }

    // Movimentos de deslize, se permitido
    if (podeSlide) {
        // Deslizes de linha
        for (int i = 0; i < 3; i++) {
            movimentos.push_back(Move(0, i, 'e'));  // Deslizar linha para a esquerda
            movimentos.push_back(Move(0, i, 'd'));  // Deslizar linha para a direita
        }
        // Deslizes de coluna
        for (int j = 0; j < 3; j++) {
            movimentos.push_back(Move(1, j, 'c'));  // Deslizar coluna para cima
            movimentos.push_back(Move(1, j, 'b'));  // Deslizar coluna para baixo
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
int win_check(Board &map) {
    // checa linhas e colunas
    for (int i = 0; i < 3; ++i) {
        if (map[i][0] != '_' && map[i][0] == map[i][1] && map[i][1] == map[i][2]) {
            return (map[i][0] == 'X') ? -1 : 1;  // -1 para X, 1 para O
        }
        if (map[0][i] != '_' && map[0][i] == map[1][i] && map[1][i] == map[2][i]) {
            return (map[0][i] == 'X') ? -1 : 1;  // -1 para X, 1 para O
        }
    }
    // checa diagonais
    if (map[0][0] != '_' && map[0][0] == map[1][1] && map[1][1] == map[2][2]) {
        return (map[0][0] == 'X') ? -1 : 1;  // -1 para X, 1 para O
    }
    if (map[0][2] != '_' && map[0][2] == map[1][1] && map[1][1] == map[2][0]) {
        return (map[0][2] == 'X') ? -1 : 1;  // -1 para X, 1 para O
    }

    // verifica se o tabuleiro está cheio (empate)
    int counter = 0;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (map[x][y] != '_') counter++;
        }
    }
    if (counter == 9) return 0; // empate
    return 2;  // jogo continua
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
int minimax(Board &map, bool isMaximizing, bool lastMoveWasSlide, double alpha, double beta) {

    double best = isMaximizing ? numeric_limits<double>::lowest() : numeric_limits<double>::max();

    int win = win_check(map);
    if (win == 1) return +1;
    if (win == -1) return -1;
    if (win == 0) return 0;

    char P = isMaximizing ? 'O' : 'X';
    auto movs = acoes(map, !lastMoveWasSlide);
    for (auto &m : movs) {
        // aplica
        if (m.type == PLACE)
            place(map, m.x, m.y, P);
        else {
            char escolha = (m.x == 0) ? 'l' : 'c';
            slide(map, escolha, m.y, m.dir);
        }
        // avalia
        double score = minimax(map, !isMaximizing, m.type == SLIDE, alpha, beta);
        // desfaz
        if (m.type == PLACE)
            map[m.x][m.y] = '_';
        else {
            char escolha = (m.x == 0) ? 'l' : 'c';
            slide(map, escolha, m.y, inverseDir(m.dir));
        }
        if (isMaximizing) {
            best = max(best, score);
            alpha = max(alpha, score);
        }
        else {
            best = min(best, score);
            beta  = min(beta, score);
        }
        if (beta <= alpha) break;
    }
    return best;
}

vector<int> Agente_joga(Board &map, bool aiLastSlide) {
    auto movs = acoes(map, !aiLastSlide);

    //Finalmente, minimax completo (inalterado)
    double bestScore = numeric_limits<double>::lowest();
    Move best(0,0);
    for (auto &m : movs) {
        if (m.type == PLACE)
            place(map, m.x, m.y, 'O');
        else {
            char escolha = (m.x == 0) ? 'l' : 'c';
            slide(map, escolha, m.y, m.dir);
        }
        double sc = minimax(map, false, m.type == SLIDE, numeric_limits<double>::lowest(), numeric_limits<double>::max());
        // desfaz
        if (m.type == PLACE)
            map[m.x][m.y] = '_';
        else {
            char escolha = (m.x == 0) ? 'l' : 'c';
            slide(map, escolha, m.y, inverseDir(m.dir));
        }
        if (sc > bestScore) {
            bestScore = sc;
            best = m;
        }
    }

    if (best.type == PLACE) {
        return {best.x, best.y};
    } else {
        return {best.x, best.y, (best.dir=='d'||best.dir=='b') ? 1 : 0};
    }
}

int main() {
    // inicializa o tabuleiro vazio
    int partidas = 0;
    vector<vector<char>> map = {
        {'_', '_', '_'},
        {'_', '_', '_'},
        {'_', '_', '_'}
    };

    vector<vector<char>> Initmap = {
        {'_', '_', '_'},
        {'_', '_', '_'},
        {'_', '_', '_'}
    };

    int seletX = 0, seletY = 0;
    char Pescolha;
    int index;
    char dir;
    bool humanLastSlide = false;
    bool aiLastSlide    = false;

    while(true){
        map = Initmap;
        partidas ++;
        cout << "\n-----------------------" << endl;
        cout << "Patida: " << partidas << endl;
        cout << "-----------------------" << endl;
 
        place(map,0,0,'O');
        cout << "Jogada da IA: 1 1" << endl;
        mostra(map);
        while (true) {
            //Vez do PLayer
            cout << "Digite 'D' para deslizar uma linha/coluna ou 'C' para colocar uma peça:" << endl;
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
                humanLastSlide = false;
            } else {
                if (humanLastSlide) {
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
                humanLastSlide = true;
            }   // checa vitória do jogador
            if (win_check(map) == -1) {
                    mostra(map);
                cout << "VOCÊ GANHOU!!!" << endl;
                break;
            }
     
            // checa vitória da IA
            if (win_check(map) == 1) {
                mostra(map);
                cout << "ROBO GANHOU!!!" << endl;
                break;
            }

            if (win_check(map) == 0) {
                mostra(map);
                cout << "EMPATE!!!" << endl;
                break;
            }
            mostra(map);

            // vez da IA
            vector<int> jogada = Agente_joga(map, aiLastSlide);
            if (jogada.size() == 2) {
                // jogada normal
                place(map, jogada[0], jogada[1], 'O');
                aiLastSlide    = false;
                cout << "Jogada da IA: " << jogada[0] + 1 << " " << jogada[1] + 1 << endl;
            } else if (jogada.size() == 3) {
                // deslize da IA
                char escolha = (jogada[0] == 0 ? 'l' : 'c');
                int idx = jogada[1];
                char idir = (jogada[2] == 0 ? (escolha == 'l' ? 'e' : 'c') : (escolha == 'l' ? 'd' : 'b'));
                slide(map, escolha, idx, idir);
                aiLastSlide    = true;
                cout << "IA desliza " << (escolha=='l'?"linha ":"coluna ") << idx+1 << " para "
                << (idir=='e'||idir=='c' ? (idir=='e'?"esquerda":"cima") : (idir=='d'?"direita":"baixo")) << endl;
            }

            if (win_check(map) == -1) {
                mostra(map);
                cout << "VOCÊ GANHOU!!!" << endl;
                break;
            }
     
            // checa vitória da IA
            if (win_check(map) == 1) {
                mostra(map);
                cout << "ROBO GANHOU. Mais sorte na próxima vez!!!" << endl;
                break;
            }

            if (win_check(map) == 0) {
                mostra(map);
                cout << "EMPATE!!!" << endl;
                break;
            }
            mostra(map);
        }
    }
    return 0;
}

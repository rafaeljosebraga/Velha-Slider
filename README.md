# 🎮 Velha-Slider

> Implementação de um _Jogo da Velha_ ampliado com operações de **slide** em linhas e colunas, e um agente de IA baseado em minimax.

## 🗂️ Índice

- [📖 Descrição](#descricao)  
- [🎬 Demonstração](#demonstracao)  
- [🚀 Funcionalidades](#funcionalidades)  
- [⚙️ Pré-requisitos](#prerequisitos)  
- [🛠️ Instalação e Compilação](#instalacao-e-compilacao)  
- [🎲 Como Jogar](#como-jogar)  
  - [✏️ Movimento de Colocar Peça](#movimento-colocar-peca)  
  - [🔄 Movimento de Slide](#movimento-slide)  
- [📜 Regras do Jogo](#regras-do-jogo)  
- [🧠 Como Funciona a IA](#como-funciona-a-ia)  
- [🗂️ Estrutura do Repositório](#estrutura-do-repositorio)

---

## <span id="descricao">📖 Descrição</span>

O projeto **Velha-Slider** é uma variação do clássico Jogo da Velha em que, além de colocar peças (`X` e `O`), o jogador e a IA podem deslizar qualquer linha ou coluna, movendo as três posições em quatro direções: esquerda, direita, para cima e para baixo. A IA utiliza o algoritmo _Minimax_, que explora recursivamente todas as sequências de jogadas possíveis até alcançar um estado terminal (vitória, derrota ou empate) e atribui pontuações a cada resultado. Para otimizar essa busca, empregamos a **poda α-β** (_alpha-beta_), que interrompe a avaliação de ramos da árvore de decisões cujos valores não podem influenciar a escolha da jogada ideal, reduzindo drasticamente o número de nós visitados sem comprometer a precisão da decisão.

## <span id="demonstracao">🎬 Demonstração</span>

```
-----------------------
Partida: 1
-----------------------
IA joga: (1,1)
O _ _
_ _ _
_ _ _
```
*(exemplo de saída inicial)*

## <span id="funcionalidades">🚀 Funcionalidades</span>

- Tabuleiro 3×3 com peças `X` (jogador) e `O` (IA).  
- Movimento de **colocar peça** em qualquer célula vazia.  
- Movimento de **slide** em linha (esquerda/direita) ou coluna (cima/baixo).  
- Agente de IA:
  - Minimax recursivo com poda α-β.  
  - Avaliação de vitória, derrota e empate.  
  - Evita dois slides consecutivos de mesmo jogador.  

## <span id="prerequisitos">⚙️ Pré-requisitos</span>

- Compilador C++ (g++, clang++ ou MSVC).  
- Sistema operacional: Windows, macOS ou Linux.  
- (Opcional) IDE/editor de sua preferência.

## <span id="instalacao-e-compilacao">🛠️ Instalação e Compilação</span>

1. Clone este repositório:
   ```bash
   git clone https://github.com/rafaeljosebraga/Velha-Slider.git
   cd Velha-Slider
   ```
2. Compile e execute:

   # Windows (cmd ou PowerShell)
   ```powershell
   cd IA
   g++ VelhaSliderIA.cpp -o VelhaSliderIA.exe
   .\VelhaSliderIA.exe
   ```
   # Linux/macOS
   ```bash
   cd IA
   g++ VelhaSliderIA.cpp -o VelhaSliderIA
   ./VelhaSliderIA
   ```

## <span id="como-jogar">🎲 Como Jogar</span>

### <span id="movimento-colocar-peca">✏️ Movimento de Colocar Peça</span>

- Digite `C` e forneça duas coordenadas `(linha, coluna)` entre 1 e 3.
- A peça `X` será colocada nesta posição se estiver vazia.

### <span id="movimento-slide">🔄 Movimento de Slide</span>

- Digite `D`, escolha `L` (linha) ou `C` (coluna), o índice (1–3) e a direção:
  - Linha: `E` para esquerda, `D` para direita.  
  - Coluna: `C` para cima, `B` para baixo.  

## <span id="regras-do-jogo">📜 Regras do Jogo</span>

- Cada jogador não pode deslizar em duas jogadas consecutivas (deve esperar um turno após um slide).  
- Caso um slide gere vitória simultânea para `X` e `O` (dupla vitória), considera-se **empate**.  

## <span id="como-funciona-a-ia">🧠 Como Funciona a IA</span>

1. Gera todas as ações possíveis (colocar + slide, se permitido naquela jogada).  
2. Para cada ação, simula recursivamente via _minimax_ com poda α-β.  
3. Avalia pontuação:
   - Vitória IA: `+10 − profundidade`  
   - Vitória jogador: `−10 + profundidade`  
   - Empate: `0`  
4. Escolhe o movimento de maior pontuação.

## <span id="estrutura-do-repositorio">🗂️ Estrutura do Repositório</span>

```
📁 IA
 ├ VelhaSliderIA.cpp              # código-fonte principal
 ├ Arvore_de_decisão.pdf          # árvore de decisão da IA após alguns passos
 ├ Jogo da velha atividade 4.pdf  # breve descrição e um exemplo de teste
 └ README.md                      # documentação do projeto
 
```
/************************************************************************************************************************************************
* DELTA EVALUATION METHOD FOR THE MINIMIZATION OF OPEN STACKS PROBLEM                                                                        *
/************************************************************************************************************************************************
* AUTHORS: JUNIOR RHIS LIMA (juniorrhis1@gmail.com) AND MARCO ANTONIO MOREIRA DE CARVALHO (marco.opt@gmail.com)                                 *
* OURO PRETO FEDERAL UNIVERSITY - UFOP - BRAZIL                                                                                                 *
* DATE OF CREATION: 01/05/2017                                                                                                                  *
* LAST MODIFICATION: 28/05/2017                                                                                                                 *
/************************************************************************************************************************************************
* TERMS OF USE                                                                                                                                  *
*                                                                                                                                               *
* This code available for free public use.                                                                                                      *
* The code in this archive is licensed gratis to all third parties under the terms of this paragraph.                                           *
* Copying and distribution of the files in this archive is unrestricted if and only if the files are not modified.                              *
* Modification of the files is encouraged, but the distribution of modifications of the files in this archive is unrestricted only              *
* if you meet the following conditions: modified files must carry a prominent notice stating (i) the author and date,                           *
* (ii) the new author and the date of release of the modification, (iii) that the work is licensed at no charge to all parties.                 *
*                                                                                                                                               *
* If you use the code extensively in your research, you are requested to provide appropriate attribution and thanks to the author of the code.  *
/************************************************************************************************************************************************
* DISCLAIMER OF WARRANTY                                                                                                                        *
*                                                                                                                                               *
* This source code is provided "as is" and without warranties as to performance or merchantability.                                             *
* The authors and/or distributors of this source code may have made statements about this source code.                                          *
* Any such statements do not constitute warranties and shall not be relied on by the user in deciding whether to use this source code.          *
*                                                                                                                                               *
* This source code is provided without any express or implied warranties whatsoever. Because of the diversity of conditions and hardware under  *
*  which this source code may be used, no warranty of fitness for a particular purpose is offered.                                              *
* The user is advised to test the source code thoroughly before relying on it. The user must assume the entire risk of using the source code.   *
*                                                                                                                                               *
************************************************************************************************************************************************/

/* Como usar: Import o arquivo 'mosp.cpp' nos includes do seu projeto
Requisitos:
	1º - Vetor com uma solução factível (permutação válida) para o problema;
	2º - Vetor com a demanda por peça do problema;
	3º - Vetor com listas de composição de peças para cada padrão da solução.
	
Implementação:
	1º - Inicialize as estruturas de uns que serão utilizadas na delta avaliação através do método 'initializeOnesStructure';
	2º - Utilize o método 'calculateDelta' para realizar a delta avaliação.
	
Obs: 
	Outras instruções estão na documentação dos métodos.
*/
#include "mosp.h"

vector <bitset<MAX>> openOnes;	// Vetor de uns abertos por estágio da solução
vector <bitset<MAX>> intermediaryOnes;	// Vetor de uns intermediários por estágio da solução
vector <bitset<MAX>> consecutiveOnes;	// Vetor de uns consecutivos por estágio da solução
vector <bitset<MAX>> closedOnes;	// Vetor de uns fechados por estágio da solução
vector <signed int> stacksAmount;	// Vetor com a quantidade de pilhas abertas por estágio da solução
bitset <MAX> uniqueDemand;	// BitSet representando as peças com demanda única na instância

/* Calcula a quantidade máxima de pilhas abertas em uma solução
Retorno:
	Quantidade máxima de pilhas abertas na solução.
	
Obs:
	O método initializeOnesStructure deve ser chamado antes para inicializar o vetor de pilhas abertas por estágio.
*/
int getMaxStacksCount() {
	return *max_element(stacksAmount.begin(), stacksAmount.end());
}

/* Inicializa a estrutura de uns abertos, intermediários, consecutivos e fechados
Parâmetros: 
	solution: Solução inicial após realização do pré-processamento por dominância;
	demand: Vetor de tamanho m, onde m representa a quantidade de peças, armazenando a demanda de cada peça em suas respectivas posições;
	patternsComposition: Vetor de tamanho n, onde n representa a quantidade de padrões, armazenando a lista de peças contidas em cada padrão em suas respectivas posições.

Retorno:
	Quantidade máxima de pilhas abertas na solução atual.
	
Obs:
	Após a primeira inicialização deve ser chamado caso haja qualquer alteração na solução que não seja realizada pelo método calculateDelta.
*/
int initializeOnesStructure(vector<signed int> solution, vector<signed int> demand, vector< list<signed int> > patternsComposition) {
	clearAll();
	
	vector<signed int> openStacks(demand.size());	// Vetor auxiliar que armazena as pilhas que vão sendo abertas
    
    openOnes.resize(solution.size());
	intermediaryOnes.resize(solution.size());
	consecutiveOnes.resize(solution.size());
	closedOnes.resize(solution.size());
	stacksAmount.resize(solution.size());
	
    int i = 0;
    int count = 0;
    
	for(vector<signed int>::iterator itd = solution.begin(); itd != solution.end(); ++itd) {
		for(list<signed int>::iterator it=patternsComposition[*itd].begin(); it != patternsComposition[*itd].end(); ++it){	// Percorre as peças que compõe o padrão
			if(openStacks[*it] == 0) {	// Verifica se uma pilha foi aberta
                openOnes[i][*it] = 1;
            } else {
            	intermediaryOnes[i][*it] = 1;
			}
			
			if(++openStacks[*it] == demand[*it]) {	// Verifica se uma pilha foi fechada (Verifica se a demanda não foi atingida)
				closedOnes[i][*it] = 1;
				intermediaryOnes[i][*it] = 0;
				openOnes[i][*it] = 0;
				
				if(demand[*it] == 1) {
					uniqueDemand[*it] = 1;
				}
            }
		}
		
		if(i > 0 && i < (int) solution.size() - 1) {
			// Adiciona as pilhas que foram abertas, são uns intermediários ou consecutivos no estágio anterior aos uns consecutivos
			// Remove as pilhas que estão fechadas ou que são uns intermediários no estado atual dos uns consecutivos
			consecutiveOnes[i] = ((openOnes[i - 1] | intermediaryOnes[i - 1] | consecutiveOnes[i - 1]) & ~intermediaryOnes[i]) & ~closedOnes[i];
		}
		
		// Calcula a quantidade de pilhas abertas no estágio atual
		stacksAmount[i] = openOnes[i].count() + intermediaryOnes[i].count() + consecutiveOnes[i].count() + closedOnes[i].count();
		
		if(stacksAmount[i] > count) {
			count = stacksAmount[i];
		}
		
		i++;
	}
	
	openStacks.clear();
	
	return count;
}

/* Realiza a delta avaliação caso o padrão de origem seja movimentado para a direita
Parâmetros:
	origin: Posição de origem do padrão;
	destiny: Posição de destino do padrão;
	openStacks: Pilhas abertas na solução atual;
	solution: Solução corrente;
	applyBestInsertion: Insere o padrão na melhor posição entre as posições de origem e destino;
	onlyImprovement: Atualiza a solução somente se a solução obtida for melhor, ou seja, delta negativo.

Retorno:
	0: Se a solução continua com a mesma quantidade de pilhas abertas;
	< 0: Indicando a quantidade de pilhas abertas a menos;
	> 0: Indicando a quantidade de pilhas abertas a mais.
*/
int calculateDeltaRight(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement) {
	int delta = 0;

	vector<bitset<MAX>> openOnesInitial(openOnes);
	vector<bitset<MAX>> intermediaryOnesInitial(intermediaryOnes);
	vector<bitset<MAX>> consecutiveOnesInitial(consecutiveOnes);
	vector<bitset<MAX>> closedOnesInitial(closedOnes);
	vector<signed int> stacksAmountInitial(stacksAmount);
	
	vector<bitset<MAX>> smallerOpenOnes;
	vector<bitset<MAX>> smallerIntermediaryOnes;
	vector<bitset<MAX>> smallerConsecutiveOnes;
	vector<bitset<MAX>> smallerClosedOnes;
	
	int smaller = openStacks;
	int bestPosition = -1;
	int currentOpenStacks = openStacks;
	
	for(int i = origin; i < destiny; i++) {
		bitset<MAX> actualOpenOnes = openOnes[i];
		bitset<MAX> actualIntermediaryOnes = intermediaryOnes[i];
		bitset<MAX> actualConsecutiveOnes = consecutiveOnes[i];
		bitset<MAX> actualClosedOnes = closedOnes[i];
		
		bitset<MAX> nextOpenOnes = openOnes[i + 1];
		bitset<MAX> nextIntermediaryOnes = intermediaryOnes[i + 1];
		bitset<MAX> nextConsecutiveOnes = consecutiveOnes[i + 1];
		bitset<MAX> nextClosedOnes = closedOnes[i + 1];
		
		bitset<MAX> intermediaryOpenAux = (actualOpenOnes & nextIntermediaryOnes);
		bitset<MAX> intermediaryClosedAux = (nextClosedOnes & actualIntermediaryOnes);
		bitset<MAX> openClosedAux = (actualOpenOnes & nextClosedOnes);
		
		openOnes[i] = nextOpenOnes | intermediaryOpenAux | openClosedAux;
		
		openOnes[i + 1] = (actualOpenOnes & ~nextIntermediaryOnes) & ~nextClosedOnes;
		
		intermediaryOnes[i] = intermediaryClosedAux | (nextIntermediaryOnes & ~actualOpenOnes);
				
		intermediaryOnes[i + 1] = intermediaryOpenAux | (actualIntermediaryOnes & ~nextClosedOnes);
		
		closedOnes[i] = (nextClosedOnes & ~actualIntermediaryOnes) & ~actualOpenOnes;
		
		closedOnes[i + 1] = actualClosedOnes | intermediaryClosedAux | openClosedAux;

		consecutiveOnes[i] = (nextConsecutiveOnes & ~actualOpenOnes) | (actualClosedOnes & ~uniqueDemand);
		
		consecutiveOnes[i + 1] = (actualConsecutiveOnes & ~nextClosedOnes) | (nextOpenOnes & ~uniqueDemand);

		stacksAmount[i] = openOnes[i].count() + intermediaryOnes[i].count() + consecutiveOnes[i].count() + closedOnes[i].count();
		stacksAmount[i + 1] = openOnes[i + 1].count() + intermediaryOnes[i + 1].count() + consecutiveOnes[i + 1].count() + closedOnes[i + 1].count();
		
		currentOpenStacks = getMaxStacksCount();
		
		if(currentOpenStacks < smaller) {
			smaller = currentOpenStacks;
			bestPosition = i + 1;
			
			smallerOpenOnes = openOnes;
			smallerClosedOnes = closedOnes;
			smallerIntermediaryOnes = intermediaryOnes;
			smallerConsecutiveOnes = consecutiveOnes;
		}
	}
	
	delta = smaller - openStacks;
	
	// Se não considerar a melhor posição calcula o delta a partir do resultado obtido pelo padrão ao chegar na posição de destino
	if(!applyBestInsertion) {
		delta = currentOpenStacks - openStacks;
	}
	
	// Desfaz as mudanças nas estruturas de uns caso haja aumento na quantidade máxima de pilhas abertas
	if(delta > 0 || (bestPosition < 0 && applyBestInsertion) || (delta == 0 && onlyImprovement)) {
		openOnes = openOnesInitial;
		intermediaryOnes = intermediaryOnesInitial;
		consecutiveOnes = consecutiveOnesInitial;
		closedOnes = closedOnesInitial;
		
		stacksAmount = stacksAmountInitial;
	
	} else {
		if(!applyBestInsertion) {	// Se não considerar a melhor posição insere na posição de destino
			bestPosition = destiny;
		}
	
		solution.insert(solution.begin() + bestPosition + 1, solution[origin]);
		solution.erase(solution.begin() + origin);
		
		if(applyBestInsertion) {
			openOnes = smallerOpenOnes;
			intermediaryOnes = smallerIntermediaryOnes;
			consecutiveOnes = smallerConsecutiveOnes;
			closedOnes = smallerClosedOnes;
		}
		
		for(int i = 0; i < (int) stacksAmount.size(); i++) {
			stacksAmount[i] = openOnes[i].count() + intermediaryOnes[i].count() + consecutiveOnes[i].count() + closedOnes[i].count();
		}
	}
	
	openOnesInitial.clear();
	intermediaryOnesInitial.clear();
	consecutiveOnesInitial.clear();
	closedOnesInitial.clear();
	stacksAmountInitial.clear();
	smallerOpenOnes.clear();
	smallerIntermediaryOnes.clear();
	smallerConsecutiveOnes.clear();
	smallerClosedOnes.clear();
	
	return delta;
}

/* Realiza a delta avaliação caso o padrão de origem seja movimentado para a direita
Parâmetros:
	origin: Posição de origem do padrão;
	destiny: Posição de destino do padrão;
	openStacks: Pilhas abertas na solução atual;
	solution: Solução corrente;
	applyBestInsertion: Insere o padrão na melhor posição entre as posições de origem e destino;
	onlyImprovement: Atualiza a solução somente se a solução obtida for melhor, ou seja, delta negativo.

Retorno:
	0: Se a solução continua com a mesma quantidade de pilhas abertas;
	< 0: Indicando a quantidade de pilhas abertas a menos;
	> 0: Indicando a quantidade de pilhas abertas a mais.
*/
int calculateDeltaLeft(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement) {
	int delta = 0;

	vector< bitset<MAX> > openOnesInitial(openOnes);
	vector< bitset<MAX> > intermediaryOnesInitial(intermediaryOnes);
	vector< bitset<MAX> > consecutiveOnesInitial(consecutiveOnes);
	vector< bitset<MAX> > closedOnesInitial(closedOnes);
	vector<signed int> stacksAmountInitial(stacksAmount);
	
	vector<bitset<MAX>> smallerOpenOnes;
	vector<bitset<MAX>> smallerIntermediaryOnes;
	vector<bitset<MAX>> smallerConsecutiveOnes;
	vector<bitset<MAX>> smallerClosedOnes;
	
	int smaller = openStacks;
	int bestPosition = -1;
	int currentOpenStacks = openStacks;
	
	for(int i = origin; i > destiny; i--) {
		bitset<MAX> actualOpenOnes = openOnes[i];
		bitset<MAX> actualIntermediaryOnes = intermediaryOnes[i];
		bitset<MAX> actualConsecutiveOnes = consecutiveOnes[i];
		bitset<MAX> actualClosedOnes = closedOnes[i];
		
		bitset<MAX> previousOpenOnes = openOnes[i - 1];
		bitset<MAX> previousIntermediaryOnes = intermediaryOnes[i - 1];
		bitset<MAX> previousConsecutiveOnes = consecutiveOnes[i - 1];
		bitset<MAX> previousClosedOnes = closedOnes[i - 1];
		
		bitset<MAX> intermediaryOpenAux = (previousOpenOnes & actualIntermediaryOnes);
		bitset<MAX> intermediaryClosedAux = (previousIntermediaryOnes & actualClosedOnes);
		bitset<MAX> openClosedAux = (previousOpenOnes & actualClosedOnes);
		
		openOnes[i] = (previousOpenOnes & ~actualIntermediaryOnes) & ~actualClosedOnes;
		
		openOnes[i - 1] = actualOpenOnes | intermediaryOpenAux | openClosedAux;
		
		intermediaryOnes[i] = intermediaryOpenAux | (previousIntermediaryOnes & ~actualClosedOnes);
		
		intermediaryOnes[i - 1] = intermediaryClosedAux | (actualIntermediaryOnes & ~previousOpenOnes);
				
		closedOnes[i] = previousClosedOnes | intermediaryClosedAux | openClosedAux;
		
		closedOnes[i - 1] = (actualClosedOnes & ~previousIntermediaryOnes) & ~previousOpenOnes;

		consecutiveOnes[i] = (previousConsecutiveOnes & ~actualClosedOnes) | (actualOpenOnes & ~uniqueDemand);
		
		consecutiveOnes[i - 1] = (actualConsecutiveOnes & ~previousOpenOnes) | (previousClosedOnes & ~uniqueDemand);

		stacksAmount[i - 1] = openOnes[i - 1].count() + intermediaryOnes[i - 1].count() + consecutiveOnes[i - 1].count() + closedOnes[i - 1].count();
		stacksAmount[i] = openOnes[i].count() + intermediaryOnes[i].count() + consecutiveOnes[i].count() + closedOnes[i].count();
		
		currentOpenStacks = getMaxStacksCount();
		
		if(currentOpenStacks < smaller) {
			smaller = currentOpenStacks;
			bestPosition = i - 1;
			
			smallerOpenOnes = openOnes;
			smallerClosedOnes = closedOnes;
			smallerIntermediaryOnes = intermediaryOnes;
			smallerConsecutiveOnes = consecutiveOnes;
		}
	}
	
	delta = smaller - openStacks;
	
	// Se não considerar a melhor posição calcula o delta a partir do resultado obtido pelo padrão ao chegar na posição de destino
	if(!applyBestInsertion) {
		delta = currentOpenStacks - openStacks;
	}
	
	if(delta > 0 || (bestPosition < 0 && applyBestInsertion) || (delta == 0 && onlyImprovement)) {
		openOnes = openOnesInitial;
		intermediaryOnes = intermediaryOnesInitial;
		consecutiveOnes = consecutiveOnesInitial;
		closedOnes = closedOnesInitial;
		
		stacksAmount = stacksAmountInitial;
	
	} else {
		if(!applyBestInsertion) {	// Se não considerar a melhor posição insere na posição de destino
			bestPosition = destiny;
		}
		
		int temp = solution[origin];
		solution.erase(solution.begin() + origin);
		solution.insert(solution.begin() + bestPosition, temp);
		
		if(applyBestInsertion) {
			openOnes = smallerOpenOnes;
			intermediaryOnes = smallerIntermediaryOnes;
			consecutiveOnes = smallerConsecutiveOnes;
			closedOnes = smallerClosedOnes;
		}
		
		for(int i = 0; i < (int) stacksAmount.size(); i++) {
			stacksAmount[i] = openOnes[i].count() + intermediaryOnes[i].count() + consecutiveOnes[i].count() + closedOnes[i].count();
		}
	}
	
	openOnesInitial.clear();
	intermediaryOnesInitial.clear();
	consecutiveOnesInitial.clear();
	closedOnesInitial.clear();
	stacksAmountInitial.clear();
	smallerOpenOnes.clear();
	smallerIntermediaryOnes.clear();
	smallerConsecutiveOnes.clear();
	smallerClosedOnes.clear();
	
	return delta;
}

/* Realiza a delta avaliação
Parâmetros:
	origin: Posição de origem do padrão;
	destiny: Posição de destino do padrão;
	openStacks: Pilhas abertas na solução atual;
	solution: Solução corrente;
	applyBestInsertion: Insere o padrão na melhor posição entre as posições de origem e destino;
	onlyImprovement: Atualiza a solução somente se a solução obtida for melhor, ou seja, delta negativo.

Retorno:
	0: Se a solução continua com a mesma quantidade de pilhas abertas;
	< 0: Indicando a quantidade de pilhas abertas a menos;
	> 0: Indicando a quantidade de pilhas abertas a mais.

Obs:
	O método já altera a solução e atualiza todas as estruturas caso o delta encontrado seja menor ou igual a zero (quando onlyImprovement = false).
*/
int calculateDelta(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement) {
	if(origin == destiny || origin < 0 || (origin > (int) solution.size() - 1) || destiny < 0 || (destiny > (int) solution.size() - 1)) {
		return 0;
	}
	
	if(origin < destiny) {
		return calculateDeltaRight(origin, destiny, openStacks, solution, applyBestInsertion, onlyImprovement);
	}
	
	return calculateDeltaLeft(origin, destiny, openStacks, solution, applyBestInsertion, onlyImprovement);
}

/* Desaloca memória das estruturas auxiliares
*/
void clearAll() {
	openOnes.clear();
	intermediaryOnes.clear();
	consecutiveOnes.clear();
	closedOnes.clear();
	stacksAmount.clear();
}
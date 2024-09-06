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

#include <vector>
#include <list>
#include <bitset>
#include <algorithm>

#define MAX 250	// Tamanho máximo dos bitsets (Deve ser maior ou igual à maior quantidade de peças da instância)

using namespace std;

int getMaxStacksCount();
int initializeOnesStructure(vector<signed int> solution, vector<signed int> demand, vector< list<signed int> > patternsComposition);
int calculateDeltaRight(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement);
int calculateDeltaLeft(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement);
int calculateDelta(int origin, int destiny, int openStacks, vector<signed int> &solution, bool applyBestInsertion, bool onlyImprovement);
void clearAll();
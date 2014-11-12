#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <cstdlib>
using namespace std;

#define LOGLEVEL 0
static unsigned int QTD_COLUNAS = 5;
static unsigned int QTD_LINHAS = 1000000;
//const static char* arquivoBinario = "data_binario";
//const static char* PLAIN_TEXT = "datas";

static string PLAIN_TEXT("datas");
static string arquivoBinario;
static string arquivoComparacao("arquivo_saida_comparacao");
int** matriz;
std::vector< std::vector<int> > lineDatas;

void toBinaryVector() {
    cout << "Iniciando toBinaryVector" << endl;

    ifstream reader(PLAIN_TEXT.c_str());
    ofstream fout;
    fout.open(arquivoBinario.c_str(), ios::out | ios::binary);

    if( reader.is_open() == true && fout.is_open() == true ){
        //
        std::string line;
        std::vector< std::vector<int>* > all_integers(QTD_LINHAS);
        std::istream_iterator<int> endIterator;
        int i = 0;
        while ( getline( reader, line ) ) {
           std::istringstream is( line );
           all_integers[i] = new std::vector<int>( std::istream_iterator<int>(is),
                                    endIterator);

           i++;
        }

        unsigned int tamanhoVetor = all_integers.size();
        for(unsigned int j = 0; j < tamanhoVetor; ++j){
            std::vector<int>* vetor = all_integers[j];
            for( unsigned int k =0; k < vetor->size(); k++){
                fout.write( (char*)&vetor->at(k), sizeof(int));
            }
        }

    }

    reader.close();
    fout.close();
    cout << "Arquivo binario criado: ok" << endl;
}

void toBinaryArrayInt() {
	std::vector<int> primeira_linha = std::vector<int>(QTD_COLUNAS+1, 0);
    cout << "Iniciando toBinaryArrayInt" << endl;

    ifstream reader(PLAIN_TEXT.c_str());
    ofstream fout;

    fout.open(arquivoBinario.c_str(), ios::out | ios::binary);

    if( reader.is_open() == true && fout.is_open() == true ){

    	// le primeir linha (1 coluna a mais)
        for(unsigned int i = 0; i < QTD_COLUNAS+1; i++)
        	reader >> primeira_linha[i];

        for(unsigned int i = 0; i < QTD_COLUNAS+1; i++)
        	fout.write((char*)&primeira_linha.at(i), sizeof(int));

        //
        std::string line;
        int **all_integers = new int*[QTD_LINHAS];
        for(unsigned int i = 0; i < QTD_LINHAS; i++)
            all_integers[i] = new int[QTD_COLUNAS];

        for(unsigned int j = 0; j < QTD_LINHAS; ++j){
            int* vetor = all_integers[j];
            for( unsigned int k =0; k < QTD_COLUNAS; ++k){
                reader >> vetor[k];
            }
        }

        /*for(unsigned int j = 0; j < QTD_LINHAS; ++j){
            fout.write((char*)all_integers[j], sizeof(int)*QTD_COLUNAS);
        }*/
        for(unsigned int j = 0; j < QTD_LINHAS; ++j){
            for( unsigned int k =0; k < QTD_COLUNAS; ++k){
            	fout.write((char*)&all_integers[j][k], sizeof(int));
            }
        }

    }

    reader.close();
    fout.close();
    cout << "Arquivo binario criado: ok" << endl;
}

void readerPlainTextCopiaPreAlocado()
{
    cout << "Iniciando readerPlainTextCopiaPreAlocado" << endl;
    clock_t inicio = clock();
    fstream reader(PLAIN_TEXT.c_str());
    std::string line;
    std::vector< std::vector<int>* > lineDatasPreAlocado(QTD_LINHAS);
    std::istream_iterator<int> endIterator;

    if( reader.is_open() == true)
    {
        int i = 0;
        while ( getline( reader, line ) ) {
           std::istringstream is( line );
           lineDatasPreAlocado[i] = new std::vector<int>( std::istream_iterator<int>(is),
                                    endIterator);

            #if LOGLEVEL >= 2
               if( lineDatasPreAlocado[i]->size() != QTD_COLUNAS ){
                   cout <<"ERRO - TAMANHO INVALIDO: lineDatas[" << i << "]:" << endl;
                   cout << "Lido: lineDatas[i]->size() = " << lineDatasPreAlocado[i]->size() << endl;
                  cout << "Esperado - Colunas: " << QTD_COLUNAS << endl;
                   cout << endl;
               }
            #endif

           i++;
        }
    }

    reader.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura texto plano (pre alocado): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;

#if LOGLEVEL >= 1
    cout <<"readerPlainTextCopiaPreAlocado : " << endl;
    cout << lineDatasPreAlocado.size() << endl;
    cout << endl;
#endif

#if LOGLEVEL >= 3
    cout <<"readerPlainText : " << endl;
    cout << "Tem que iterar o vetor - depois faco" << endl;
    cout << endl;
#endif
}

void readerPlainTextCopiaPorValor()
{
    cout << "Iniciando readerPlainTextCopiaPorValor" << endl;
    clock_t inicio = clock();
    fstream reader(PLAIN_TEXT.c_str());
    std::string line;
    std::istream_iterator<int> endIterator;

    if( reader.is_open() == true)
    {
        int i = 0;
        while ( getline( reader, line ) ) {
           std::istringstream is( line );
           lineDatas[i] = std::vector<int>( std::istream_iterator<int>(is),
                                   endIterator );

           i++;
        }
    }

    reader.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura texto plano (copia por valor): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;

#if LOGLEVEL >= 1
    cout <<"readerPlainTextCopiaPorValor : " << endl;
    cout << lineDatas.size() << endl;
    cout << endl;
#endif

#if LOGLEVEL >= 3
    cout <<"readerPlainText : " << endl;
    cout << "Tem que iterar o vetor - depois faco" << endl;
    cout << endl;
#endif
}

void readerBinaryFileMatrizJaAlocada()
{
    cout << "Iniciando readerBinaryFileMatrizJaAlocada" << endl;
    clock_t inicio = clock();
    fstream inputStream;
    inputStream.open(arquivoBinario.c_str(), ios::in | ios::binary);

    if( inputStream.is_open() == true)
    {
        for( unsigned int i = 0; i < QTD_LINHAS; i++){
            inputStream.read((char*)matriz[i], QTD_COLUNAS*sizeof(int));
        }
    }

    inputStream.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura binaria (readerBinaryFileMatrizJaAlocada): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;
}

void readerBinaryFileAlocandoMatriz()
{
    cout << "Alocando Matriz " << endl;
    clock_t inicio = clock();

    matriz = new int*[QTD_LINHAS];
    for(unsigned int i = 0; i < QTD_LINHAS; i++)
        matriz[i] = new int[QTD_COLUNAS];

    fstream inputStream;
    inputStream.open(arquivoBinario.c_str(), ios::in | ios::binary);

    if( inputStream.is_open() == true)
    {
        for( unsigned int i = 0; i < QTD_LINHAS; i++){
            inputStream.read((char*)matriz[i], QTD_COLUNAS*sizeof(int));
        }
    }

    inputStream.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura binaria (readerBinaryFileAlocandoMatriz): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;
}

void saveBinaryFileAsText(){
    cout << "Iniciando escrita arquivo de comparacao" << endl;

    ofstream fout(arquivoComparacao.c_str());

    for( unsigned int i = 0; i < QTD_LINHAS; i++){
        for( unsigned int j = 0; j < QTD_COLUNAS; j++){
            fout << matriz[i][j];
            if( j != QTD_COLUNAS -1)
                fout << " ";
        }
        if( i != QTD_LINHAS -1)
            fout << endl;
    }

    cout << "arquivo de comparacao: arquivo_saida_comparacao ok" << endl;
}

void desalocaMatriz(){
    cout << "Desalocando Matriz" << endl;
    for(unsigned int i = 0; i < QTD_LINHAS; i++)
        delete matriz[i];

    delete matriz;
}

void readerBinaryFileMatrizJaAlocadaLendoNumeroANumero()
{
    cout << "Iniciando readerBinaryFileMatrizJaAlocadaLendoNumeroANumero" << endl;
    clock_t inicio = clock();
    fstream inputStream;
    inputStream.open(arquivoBinario.c_str(), ios::in | ios::binary);

    if( inputStream.is_open() == true)
    {
        for( unsigned int i = 0; i < QTD_LINHAS; i++){
        	for( unsigned int j = 0; j < QTD_COLUNAS; j++){
        		inputStream.read((char*)&matriz[i][j], sizeof(int));
        	}
        }
    }

    inputStream.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura binaria (readerBinaryFileMatrizJaAlocada): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;
}

void readerBinaryFileAlocandoMatrizLendoNumeroANumero()
{
	cout << "Iniciando readerBinaryFileAlocandoMatrizLendoNumeroANumero" << endl;
	cout << "Alocando Matriz " << endl;
    clock_t inicio = clock();

    matriz = new int*[QTD_LINHAS];
    for(unsigned int i = 0; i < QTD_LINHAS; i++)
        matriz[i] = new int[QTD_COLUNAS];

    fstream inputStream;
    inputStream.open(arquivoBinario.c_str(), ios::in | ios::binary);

    if( inputStream.is_open() == true)
    {
        for( unsigned int i = 0; i < QTD_LINHAS; i++){
        	for( unsigned int j = 0; j < QTD_COLUNAS; j++){
        		inputStream.read((char*)&matriz[i][j], sizeof(int));
        	}
        }
    }

    inputStream.close();
    clock_t tempo = clock() - inicio;
    cout << "Leitura binaria (readerBinaryFileAlocandoMatriz): " <<
            (float)tempo/CLOCKS_PER_SEC << " segundos" << endl;
}


int main(int argc, char**argv)
{
    cout << "Forma de uso:" << endl;
    cout << "executavel <colunas> <linhas> <nome_arquivo_entrada>" << endl;

    if( argc >= 2){
        // le coluna
        QTD_COLUNAS = atoi(argv[1]);
    }

    if( argc >= 3){
        // le coluna
        QTD_LINHAS = atoi(argv[2]);
    }

    if( argc >= 4){
        // le arquivo
        PLAIN_TEXT.assign(argv[3]);
    }

    cout << "Colunas: " << QTD_COLUNAS << endl;
    cout << "Linhas: " << QTD_LINHAS << endl;
    cout << "Arquivo entrada: " << PLAIN_TEXT << endl;

    arquivoBinario.assign(PLAIN_TEXT);
    arquivoBinario.append("_binario");

    arquivoComparacao.assign(PLAIN_TEXT);
    arquivoComparacao.append("_comparacao");

    cout << "Arquivo binario saida: " << arquivoBinario << endl;
    cout << "Arquivo comparacao saida: " << arquivoComparacao << endl;

    lineDatas = std::vector< std::vector<int> >(QTD_LINHAS);

    toBinaryArrayInt();
//    toBinaryVector();
/*    readerPlainTextCopiaPorValor();
    readerPlainTextCopiaPreAlocado();

    readerBinaryFileAlocandoMatriz();
    readerBinaryFileMatrizJaAlocada();

    desalocaMatriz();

    readerBinaryFileAlocandoMatrizLendoNumeroANumero();
    readerBinaryFileMatrizJaAlocadaLendoNumeroANumero();

    saveBinaryFileAsText();*/

    return 0;
}


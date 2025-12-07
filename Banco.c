#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *ARQUIVO_CONTA = "contas.dat";
const char *SENHA_ADMIN = "teste123";

struct Conta {
    char titular[50];
    int numero;
    float saldo;
};

void criarConta();
void extrato();
void depositar();
void sacar();
void transferencia();
void listarContas();
int autenticarAdmin();
int carregarultimoNumeroConta();
void salvarConta(struct Conta conta);
void buscarConta(int numero, struct Conta *conta);
void atualizarConta(struct Conta conta);

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int opcao; // Com acentuação pode dar erro nesse krl

    do {

        printf ("\n Serviço Bancário \n");
        printf("1. Criar Conta\n");
        printf("2. Extrato\n");
        printf("3. Depositar\n");
        printf("4. Sacar\n");
        printf("5. Transferência\n");
        printf("6. Listar Contas (Admin)\n");
        printf("7. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                criarConta();
                break;
            case 2:
                extrato();
                break;
            case 3:
                depositar();
                break;
            case 4:
                sacar();
                break;
            case 5:
                transferencia();
                break;
            case 6:
                if (autenticarAdmin()) {
                    listarContas();
                } else {
                    printf("Autenticação falhou. Acesso negado.\n");
                }
                break;
            case 7:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 7);
    return 0;
    }

    void criarConta(void) {
        struct Conta novaconta;
        
        printf ("Digite o nome do titular:");
        fflush (stdout);
        fgets (novaconta.titular, sizeof(novaconta.titular), stdin);

        novaconta.titular[strcspn(novaconta.titular, "\n")] = '\0'; 
        novaconta.numero = carregarultimoNumeroConta() + 1;
        novaconta.saldo = 0.0f;
        salvarConta(novaconta);
        
        printf("Conta criada com sucesso! Número da conta: %d\n", novaconta.numero);

        }
    void extrato(void) {
        struct Conta conta;
        int numero;

        printf ("Extrato:\n");
        scanf ("%d", &numero);
        limparBuffer();
        buscarConta(numero, &conta);

        if (conta.numero != 0) {
            printf ("Titular: %s\n", conta.titular);
            printf ("Número da Conta: %d\n", conta.numero);
            printf ("Saldo: R$ %.2f\n", conta.saldo);
        } else {
            printf ("Conta não encontrada.\n");
        }    
    }
        void depositar(void) {
        struct Conta conta;
        int numero;
        float valor;

        printf ("Depósito:\n");
        scanf ("%d", &numero);
        limparBuffer();
        buscarConta(numero, &conta);

        if (conta.numero != 0) {
            printf ("Digite a quantia a depositar: ");
            scanf("%f", &valor);
        }
            else {
                printf ("Conta não encontrada.\n");
            }
        }
        void sacar(void) {
            struct Conta conta;
            int numero;
            float valor;

            printf ("Saque: \n");
            scanf ("%d", &numero);
            limparBuffer();
            buscarConta(numero, &conta);

            if (conta.numero != 0) {
                printf ("Digite a quantia desejada para saque: ");
                scanf("%f", &valor);
            }
        }
        void transferencia(void) {
            struct Conta contaOrigem, contaDestino;
            int numeroOrigem, numeroDestino;
            float valor;

            printf ("Transferência:\n");
            printf ("Digite o número da conta de origem: ");
            scanf ("%d", &numeroOrigem);
            limparBuffer();
            buscarConta(numeroOrigem, &contaOrigem);
            if (contaOrigem.numero != 0) {
                printf ("Digite o número da conta de destino: ");
                scanf ("%d", &numeroDestino);
                limparBuffer();
                buscarConta(numeroDestino, &contaDestino);

                if (contaDestino.numero != 0) {
                    printf ("Digite a quantia a transferir: ");
                    scanf ("%f", &valor);
                } else {
                    printf ("Conta de destino não encontrada.\n");
                }
            } else {
                printf ("Conta de origem não encontrada.\n");
            }
        }
        void listarContas(void) {
            struct Conta conta;
            FILE *file = fopen(ARQUIVO_CONTA, "rb");
            if (file == NULL) {
                printf("Nenhuma conta cadastrada.\n");
                return;
            }

            printf("Listagem de Contas:\n");
            while (fread(&conta, sizeof(struct Conta), 1, file)) {
                printf("Número: %d, Titular: %s, Saldo: R$ %.2f\n", conta.numero, conta.titular, conta.saldo);
            }
            fclose(file);
        }
        int autenticarAdmin(void) {
            char senha[50];
            printf("Digite a senha de administrador: ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = '\0';

            return strcmp(senha, SENHA_ADMIN) == 0;
        }
        int carregarultimoNumeroConta(void) {
            struct Conta conta;
            FILE *file = fopen(ARQUIVO_CONTA, "rb");
            int ultimoNumero = 0;

            if (file == NULL) {
                return 0;
            }

            while (fread(&conta, sizeof(struct Conta), 1, file)) {
                if (conta.numero > ultimoNumero) {
                    ultimoNumero = conta.numero;
                }
            }
            fclose(file);
            return ultimoNumero;
        }
        void salvarConta(struct Conta conta) {
            FILE *file = fopen(ARQUIVO_CONTA, "ab");
            if (file != NULL) {
                fwrite(&conta, sizeof(struct Conta), 1, file);
                fclose(file);
            }
        }
        void buscarConta(int numero, struct Conta *conta) {
            FILE *file = fopen(ARQUIVO_CONTA, "rb");
            conta->numero = 0;  
            if (file == NULL) {
                return;
            }
            while (fread(conta, sizeof(struct Conta), 1, file)) {
                if (conta->numero == numero) {
                    fclose(file);
                    return;
                }
            }
            fclose(file);
        }
        void atualizarConta(struct Conta conta) {
            FILE *file = fopen(ARQUIVO_CONTA, "r+b");
            struct Conta temp;  
            if (file == NULL) {
                return;
            }
            while (fread(&temp, sizeof(struct Conta), 1, file)) {
                if (temp.numero == conta.numero) {
                    fseek(file, -sizeof(struct Conta), SEEK_CUR);
                    fwrite(&conta, sizeof(struct Conta), 1, file);
                    break;
                }
            }
            fclose(file);
        }   
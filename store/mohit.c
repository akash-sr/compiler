#include <stdio.h>
#include "parser.h"
#include "parserDef.h"

void grammar_fill(FILE *fp)
{
    // grammar ek array he (cell type ka).
    // har index par ek grammar rule ki information store he.
    // 1. lhs of grmmar,
    // 2. first symbol on rhs
    // 3. rlast symbol on rhs
    int rule_num = 0;//jab bhi koi file aayegi grammar ki toh uske saare grammars aayege. toh rule_num globally define karne ki jarurat nahi he. isi function me local variable bnakar grammar array ko index kar lege
    char buffer[RHS_MAX_LENGTH];//hum ek iteration me ek hi rule read karege. toh buffer ki size define bhi utni hi ki

    while (fgets(buffer, sizeof(buffer), fptr) != NULL) // fgets keeps on reading until new line is encountered or maximum limit of character array. toh ek baar me hum ek hi rule read karege grammar.txt se
    {
        char *sym_read; //yeh strtok dwara tokenize kiye hue symbol ko point karega

        //????strtok ek baar me ek hi token deta he ???????
        sym_read = strtok(buffer, " \n");//pehla symbol tokenize kar diya

        //humne ek hi rule read kiya he, toh iss while loop ke ek iteration me sabse pehla wala symbol lhs ho hoga. baad me koi bhi lhs nahi aayega
        // sabse pehla token toh lhs hoga, jo ki nonterminal he
        // toh usko assign kar dete he idhar
        //get_symbol symbol return karega. and grammar.lhs bhi "symbol" type ka he
        grammar[rule_num].lhs = get_symbol(sym_read).nt; // symbol me union ke andar 2 field he -> nt/t. since, lhs nt hi hota he, toh symbol.nt kar diya idhar
        grammar[rule_num].head = NULL;
        grammar[rule_num].tail = NULL;
        sym_read = strtok(NULL, " \n");//ab aage wale tokens(symbols) dekhte he

        while (sym_read != NULL)
        {
            symbol sym = get_symbol(sym_read);
            //symbol ko linked list ke end me insert karte jayege
            insert_at_end(&(grammar[rule_num].tail), sym);//rule_num naam ka variable yha kaam ayega grammar array ko index karne ke liye
            if (grammar[rule_num].head == NULL)//hum head ko pehli baari me hi tail ke bara bar kar denge. pehli baari me tail bhi first symbol ko hi point karega. isliye yeh check lgaya he. baad wali iterations me head ko kuch nahi hoga.
            {
                grammar[rule_num].head = grammar[rule_num].tail;
            }
            sym_read = strtok(NULL, " \n");
        }

        // for (int i = 0; sym_read != NULL; i++)
        // {
        //     if (i == 0) // LHS of a production
        //     {
        //          grammar[rule_num].lhs = get_symbol(sym_read).nt;//symbol me union ke andar 2 field he - nt/t. since, lhs nt hi hota he, toh symbol.nt kar diya idhar
        //          grammar[rule_num].head = NULL;
        //          grammar[rule_num].tail = NULL;
        //     }
        //     else
        //     {
        //         symbol sym = get_symbol(sym_read);
        //         insert_at_end(&(grammar[rule_num].tail), sym);
        //         if (grammar[rule_num].head == NULL)
        //         {
        //             grammar[rule_num].head = grammar[rule_num].tail;
        //         }
        //     }
        //     sym_read = strtok(NULL, " \n");
        // }

        rule_num++;//akhri kar humne ek rule padh hi liya pura, ab agli baar next rule padhenge. toh rule number badha diya
    }
}

void print_symbol(symbol sym)
{                                // symbol yaa toh terminal hoga yaa nonterminal
    if (sym.is_terminal != true) // agar non_terminal he
    {
        printf("%s", non_terminal_string[sym.nt]); // nonterminal ek enum he, toh symbol.nt ek number return karega, jisko  use karke nonterminal string array ko index kar sakte he
    }
    else // agar terminal he
    {
        printf("%s", terminal_string[sym.t]); // terminal "tokenname" type ka he. tokename ek enum he. Toh sym.t ek number return karega. terminal string ~ keytotoken
    }
}

// void print_rule(int rule_no)
// {                                               // takes rule# as input parameter and print that grammar rule
//     int lhs = grammar[rule_no].lhs;             // grammer cell type ka he, cell.lhs ek nonterminal naam ka enum he. isliye lhs ek int type define kiya he idhar
//     rhsnode_ptr head = grammar[rule_no].head;   // grammar.head rhsnode_ptr type ka he. rhsnode_ptr me symbol type ki field he. toh head->symbol pass kar denge as a parementer to print_symbol
//     printf("%s -> ", non_terminal_string[lhs]); // grammar ka lhs print kar diya
//     while (head != NULL)                        // ab grammar ka rhs print karna chalu kiya he
//     {
//         print_symbol(head->sym); // head grammar ke rhs ki linked list ko point kar rha he. isko use karke uss rule ka rhs print kar sakte he
//         printf(" ");
//         head = head->next;
//     }
//     printf("\n");
// }

void print_grammar()
{
    for (int i = 0; i < NUM_OF_RULES; i++)
    {
        // print_rule(i); // ek ek karke rule no ke hisab se rules print kar rahe he. print_rule int type ka parameter accept karta he. jo ki rule no he.
        int lhs = grammar[i].lhs;             // grammer cell type ka he, cell.lhs ek nonterminal naam ka enum he. isliye lhs ek int type define kiya he idhar
        rhsnode_ptr head = grammar[i].head;   // grammar.head rhsnode_ptr type ka he. rhsnode_ptr me symbol type ki field he. toh head->symbol pass kar denge as a parementer to print_symbol
        printf("%s -> ", non_terminal_string[lhs]); // grammar ka lhs print kar diya
        while (head != NULL)                        // ab grammar ka rhs print karna chalu kiya he
        {
            print_symbol(head->sym); // head grammar ke rhs ki linked list ko point kar rha he. isko use karke uss rule ka rhs print kar sakte he
            printf(" ");
            head = head->next;
        }
        printf("\n");
    }
}

void free_grammar()
{
    int i;
    for (i = 0; i < NUM_OF_RULES; i++) // ek loop me ek grammer rule ko free kar denge
    {
        rhsnode_ptr temp = grammar[i].head; // grammer ek cell type ka array he, jo ki humari grammar store karta he
        // iss grammar me head points to first symbol on rhs of production rule
        // rhsnode_ptr ek struct he jo production rule ka rhs side store karta he(ek linked list ke form me)
        rhsnode_ptr prev;
        while (temp != NULL)
        {
            prev = temp->next; // free karne se pehle uske agle wale symbol ko access kar liya
            free(temp);        // isko free kar diya
            temp = prev;
        }
    }
}

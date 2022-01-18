/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 */
/* Codigo:Soma os elementos de um vetor usando threads em Java */


//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
    //recurso compartilhado
    private int r;
    //construtor
    public S() { 
       this.r = 0; 
    }
  
    
    public void inc(int a) { 
       this.r += a; 
    }
    
    public int get() { 
       return this.r; 
    }
    
    /*
    //operacao de escrita sobre o recurso compartilhado
    public synchronized void inc(int a) { 
       this.r += a; 
    }
    
    //operacao de leitura sobre o recurso compartilhado
    public synchronized int get() { 
        return this.r; 
    }
    */
  }
  
  //classe que estende Thread e implementa a tarefa de cada thread do programa 
  class T extends Thread {
     //identificador da thread
     private int id;
     //objeto compartilhado com outras threads (valor do )
     S s;
    
     //construtor
     public T(int tid, S s) { 
        this.id = tid; 
        this.s = s;
     }
  
     //metodo main da thread
     public void run() {
        System.out.println("Thread " + this.id + " iniciou!");
        //incrementação da variável 
        for (int i= this.id; i<TSomaVetor.vetor.length; i += TSomaVetor.N) {
           this.s.inc(TSomaVetor.vetor[i]);  
        }
        System.out.println("Thread " + this.id + " terminou!"); 
     }
  }
  
  //classe da aplicacao
  class TSomaVetor {
     public static final int N = 3;
     public static int vetor[] = new int[100];
     public static void main (String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[N];
  
        //cria uma instancia do recurso compartilhado entre as threads
        S s = new S();
  
       
        //inicializa o vetor, colocando o valor de i+1 na posição i
        for (int i = 0; i<vetor.length;i++){
            vetor[i] = i + 1;
        }
            

        //cria as threads da aplicacao
        for (int i=0; i<threads.length; i++) {
           threads[i] = new T(i, s);
        }
  
        //inicia as threads
        for (int i=0; i<threads.length; i++) {
           threads[i].start();
        }
  
        //espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
           try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
  
        System.out.println("Valor de s = " + s.get()); 
     }
  }

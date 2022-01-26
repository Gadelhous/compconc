/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Lab 8 :Implementaçãoo de uma aplicação que usa o padrão leitores/escritores */
/* -------------------------------------------------------------------*/

class LE {
    private int leit, escr;  
    
    // Construtor
    LE() { 
       this.leit = 0; //leitores lendo (0 ou mais)
       this.escr = 0; //escritor escrevendo (0 ou 1)
    } 
    
    // Entrada para leitores
    public synchronized void EntraLeitor (int id) {
      try { 
        while (this.escr > 0) {
        //if (this.escr > 0) {
           System.out.println ("le.leitorBloqueado("+id+")");
           wait();  //bloqueia pela condicao logica da aplicacao 
        }
        this.leit++;  //registra que ha mais um leitor lendo
        System.out.println ("le.leitorLendo("+id+")");
      } catch (InterruptedException e) { }
    }
    
    // Saida para leitores
    public synchronized void SaiLeitor (int id) {
       this.leit--; //registra que um leitor saiu
       if (this.leit == 0) 
             this.notify(); //libera escritor (caso exista escritor bloqueado)
       System.out.println ("le.leitorSaindo("+id+")");
    }
    
    // Entrada para escritores
    public synchronized void EntraEscritor (int id) {
      try { 
        while ((this.leit > 0) || (this.escr > 0)) {
        //if ((this.leit > 0) || (this.escr > 0)) {
           System.out.println ("le.escritorBloqueado("+id+")");
           wait();  //bloqueia pela condicao logica da aplicacao 
        }
        this.escr++; //registra que ha um escritor escrevendo
        System.out.println ("le.escritorEscrevendo("+id+")");
      } catch (InterruptedException e) { }
    }
    
    // Saida para escritores
    public synchronized void SaiEscritor (int id) {
       this.escr--; //registra que o escritor saiu
       notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
       System.out.println ("le.escritorSaindo("+id+")");
    }
}



//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Leitor (int id, int delayTime, LE m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                lab8.ehPar();
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay); 
            }
        } catch (InterruptedException e) { return; }
    }
}
  
//--------------------------------------------------------
// Escritor
class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Escritor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id); 
                lab8.trocaVariavel(this.id);    
                this.monitor.SaiEscritor(this.id); 
                sleep(this.delay); 
            }
      } catch (InterruptedException e) { return; }
    }
}

//--------------------------------------------------------
// Leitor e Escritor
class LEscritor extends Thread {
    int id;
    int delay;
    LE monitor;

    // Construtor
    LEscritor (int id, int delayTime, LE m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }
    
    // Método executado pela thread
    public void run () {
        double j=777777777.7, i;
        try {
          for (;;) {
                this.monitor.EntraLeitor(this.id);
                lab8.printaVariavel();
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay); 
                for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de escrita
                this.monitor.EntraEscritor(this.id); 
                lab8.accVariavel();
                this.monitor.SaiEscritor(this.id); 
                sleep(this.delay); //atraso bobo...
          }
        } catch (InterruptedException e) { return; }
      }
}

//--------------------------------------------------------
// Classe principal
class lab8{
    static final int L = 4;
    static final int E = 3;
    static final int LESC = 3;  //Por conta da classe LE consideramos o númeor de threads leitoras e escritoras como LESC
    static int variavel = 0;
  
    public static void ehPar(){
        if(variavel % 2 == 0){
            System.out.println("O número é par");
        }else{
            System.out.println("O número é ímpar");
        }
    }

    public static void accVariavel(){
        variavel++;
    }

    public static void trocaVariavel(int a){
        variavel = a;
    }

    public static void printaVariavel(){
        System.out.println(variavel);
    }
    public static void main (String[] args) {
        int i;
        LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
        Leitor[] l = new Leitor[L];       // Threads leitores
        Escritor[] e = new Escritor[E];   // Threads escritores
        LEscritor[] lEsc = new LEscritor[LESC];       // Threads leitoras/escritoras
    
        //inicia o log de saida
        System.out.println ("import verificaLE");
        System.out.println ("le = verificaLE.LE()");
        
        for (i=0; i<L; i++) {
            l[i] = new Leitor(i+1, (i+1)*500, monitor);
            l[i].start(); 
        }

        for (i=0; i<E; i++) {
            e[i] = new Escritor(i+1, (i+1)*500, monitor);
            e[i].start(); 
        }

        for(i = 0; i<LESC; i++){
            lEsc[i] = new LEscritor(i+1, (i+1)*500, monitor);
            lEsc[i].start();
        }
    }
}

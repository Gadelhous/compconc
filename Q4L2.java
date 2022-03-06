import java.util.concurrent.PriorityBlockingQueue;

//Queue/PriorityQueue/PriorityBlockingQueue
class LE {
    private int leit, escr,limite;

    // Construtor
    LE() {
        this.leit = 0; //Impressora lendo (0 ou mais)
        this.escr = 0; //Requisitora escrevendo (0 ou 1)
        this.limite = 0;
    }

    // Entrada para impressora
    public synchronized void EntraLeitor (int id) {
        try {
            while (Q4Lista2.contador == 0) {
                System.out.println ("le.ImpressorBloqueado("+id+") : A fila de impressão está vazia!" );
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.leit++;  //registra que ha mais um leitor lendo
            //System.out.println ("le.ImpressorLendo("+id+")");
        } catch (InterruptedException e) { }
    }

    // Saida para leitores
    public synchronized void SaiLeitor (int id) {
        this.leit--; //registra que um leitor saiu
        //System.out.println ("le.ImpressorSaindo("+id+")");
    }

    // Entrada para escritores
    public synchronized void EntraEscritor (int id) {
        try {
            while ((Q4Lista2.contador > Q4Lista2.fila.size())) {
                System.out.println ("le.requisitorBloqueado("+id+") : A fila está cheia!");
                wait();  //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            //System.out.println ("le.requisitorRequisitando("+id+")");
        } catch (InterruptedException e) { e.printStackTrace(); }
    }

    // Saida para escritores
    public synchronized void SaiEscritor (int id) {
        this.escr--; //registra que o requisitor saiu
        notify(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        //System.out.println ("le.RequisitorSaindo("+id+")");
    }

}



//Aplicacao de exemplo--------------------------------------------------------
// Requisitora
class Requisitora extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads


    // Construtor
    Requisitora(int id, int delayTime, LE m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {
        try {
            for (;;) {
                this.monitor.EntraEscritor(this.id);
                this.requisita(" x ");
                Q4Lista2.contador++;
                this.monitor.SaiEscritor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
            return; }
    }

    public void requisita(String r){
        //System.out.println("Eu, a requisitora " +this.id+ " estou adicionando uma requisição na fila");
        Q4Lista2.fila.add(r);
        Q4Lista2.contador++;
    }

}

//--------------------------------------------------------
// Impressora
class Impressora extends Thread {
    int id;
    int delay;
    LE monitor;

    // Construtor
    Impressora (int id, int delayTime, LE m) {
        this.id = id;
        this.delay = delayTime;
        this.monitor = m;
    }

    // Método executado pela thread
    public void run () {

        try {
            for (;;) {
                this.monitor.EntraLeitor(this.id);
                this.imprime();
                this.monitor.SaiLeitor(this.id);
                sleep(this.delay);
            }
        } catch (InterruptedException e) { e.printStackTrace(); }
    }

    private void imprime(){
        System.out.println("\nEstou Imprimindo");
        Q4Lista2.fila.poll();
        Q4Lista2.contador--;
        notify();
    }
}

//--------------------------------------------------------
// Classe principal
class Q4Lista2{

    static final int E = 3; //E = Requisitora
    static final int LESC = 1;  //LESC = Impressora;

    static int tamanho_máximo= 5;
    public static int contador = 0;


    static PriorityBlockingQueue<String> fila
            = new PriorityBlockingQueue<String>();



    public void setFila(PriorityBlockingQueue<String> fila) {
        this.fila = fila;
    }




    public static void main (String[] args) {
        int i;
        LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
        Requisitora[] e = new Requisitora[E];   // Threads escritores
        Impressora[] lEsc = new Impressora[LESC];       // Threads leitoras/escritoras

        /*inicia o log de saida
        System.out.println ("import verificaLE");
        System.out.println ("le = verificaLE.LE()");
        */


        for (i=0; i<E; i++) {
            e[i] = new Requisitora(i+1, (i+1)*500, monitor);
            e[i].start();
        }

        for(i = 0; i<LESC; i++){
            lEsc[i] = new Impressora(i+1, (i+1)*500, monitor);
            lEsc[i].start();
        }
    }
}

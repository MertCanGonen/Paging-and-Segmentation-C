//Mert Can Gönen
//181101039
//Bil 461 - Homework 3

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv){
    
    char * seg0 = argv[1];
    char * seg1 = argv[2];
    char * seg2 = argv[3];

    int segment0 = atoi(seg0); //segment0 (byte olarak)
    int segment1 = atoi(seg1); //segment1 (byte olarak)
    int segment2 = atoi(seg2); //segment2 (byte olarak)

    //SEGMENT 0
    int segment0Pages = segment0 / 1024;  //segment0'in page sayisi
    if (segment0 % 1024 != 0){
        segment0Pages++;
    }
    int s0 = segment0Pages / 2;           //segment0'a ait page table'in satir sayisi
    if (segment0Pages % 2 != 0){
        s0++;
    }
    int pageTableForS0[s0][2]; //pageTableForS0[x][0] = page number; pageTableForS0[0][1] = last used time;
    int a;
    int b;
    for (a=0; a<s0; a++){
        for (b=0; b<2; b++){
            pageTableForS0[a][b] = -1;
        }
    }
    pageTableForS0[0][0] = 0;
    pageTableForS0[0][1] = 0;


    //SEGMENT 1
    int segment1Pages = segment1 / 1024;  //segment1'in page sayisi
    if (segment1 % 1024 != 0){
        segment1Pages++;
    }
    int s1 = segment1Pages / 2;           //segment1'e ait page table'in satir sayisi
    if (segment1Pages % 2 != 0){
        s1++;
    }
    int pageTableForS1[s1][2]; //Page table for segment 1
    for (a=0; a<s1; a++){
        for (b=0; b<2; b++){
            pageTableForS1[a][b] = -1;
        }
    }
    pageTableForS1[0][0] = 0;
    pageTableForS1[0][1] = 0;

    //SEGMENT 2
    int segment2Pages = segment2 / 1024;  //segment2'nin page sayisi
    if (segment2 % 1024 != 0){
        segment2Pages++;
    }
    int s2 = segment2Pages / 2;           //segment2'ye ait page table'in satir sayisi
    if (segment2Pages % 2 != 0){
        s2++;
    }
    int pageTableForS2[s2][2]; //Page table for segment 2 
    for (a=0; a<s2; a++){
        for (b=0; b<2; b++){
            pageTableForS2[a][b] = -1;
        }
    }
    pageTableForS2[0][0] = 0;
    pageTableForS2[0][1] = 0;

    //Segment sayilarina ait sayfa sayilarinin ciktisi
    printf("Segment 0'a ait page sayisi : %d\n", segment0Pages);
    printf("Segment 1'e ait page sayisi : %d\n", segment1Pages);
    printf("Segment 2'ye ait page sayisi: %d\n", segment2Pages);

    int TLB[50][2];   //TLB[x][0] = segment number; TLB[x][1] = page number;
    int i;
    int j;
    for (i=0; i<50; i++){
        for (j=0; j<2; j++){
            TLB[i][j] = -1; 
        }
    }
    //Page table'lar ve TLB olusturuldu.

    int totalHit = 0;
    int totalMiss = 0;
    int totalFault = 0; 
    int invalidReference = 0;
    int time = 1;  //segmentlere ait page tableda uygulanan LRU algoritması icin, her page tabledaki 2.column'a (indexi 1 olan column) ait olan eleman
    
    FILE* file = fopen("reference.txt", "r");   //reference.txt dosyasindan segment ve page number'lar okundu.
    char line[50];
    char * reference;
    while (fgets(line, sizeof(line), file)) {

        reference = line;
        char * t = strtok(reference, " ");
        int seg = atoi(t);  //segment number
        t = strtok(NULL, " ");
        int page = atoi(t); //page number
        int referenceCheck = 0;  //reference hatali mi, degil mi?

        //invalid reference kontrol
        if (seg<0 || seg>2){   //segment no hatali
            invalidReference++;
            referenceCheck = -1;
        }else{                 //page no hatali
            if (seg == 0){
                if (page<0 || segment0Pages<=page){
                    invalidReference++;
                    referenceCheck = -1;
                }
            }else if (seg == 1){
                if (page<0 || segment1Pages<=page){
                    invalidReference++;
                    referenceCheck = -1;
                }
            }else if (seg == 2){
                if (page<0 || segment2Pages<=page){
                    invalidReference++;
                    referenceCheck = -1;
                }
            }
        }

        if (referenceCheck == 0){ //reference hatasiz.
            //TLB kontrol
            int i;
            int checkTLB = -1;  //TLB'de var mi, yok mu?
            for (i=0; i<50; i++){
                if (TLB[i][0] == seg && TLB[i][1] == page){
                    //TLB hit
                    totalHit++;
                    checkTLB = 0;
                    if (seg == 0){  //TLB hit, page table'da son erisildigi zamani guncelle.
                        int n;
                        for (n=0;n<s0;n++){
                            if (page == pageTableForS0[n][0]){
                                pageTableForS0[n][1] = time;
                                break;
                            }
                        }
                    }else if (seg == 1){
                        int n;
                        for (n=0;n<s1;n++){
                            if (page == pageTableForS1[n][0]){
                                pageTableForS1[n][1] = time;
                                break;
                            }
                        }
                    }else if (seg == 2){
                        int n;
                        for (n=0;n<s2;n++){
                            if (page == pageTableForS2[n][0]){
                                pageTableForS2[n][1] = time;
                                break;
                            }
                        }
                    }
                    usleep(1000);
                    break;
                }
            }

            if (checkTLB == -1){ //TLB miss, page table'ı kontrol et (segment'e ait)
                totalMiss++;
                int checkPageTable = -1; //page table'da var mi, yok mu?
                if (seg == 0){
                    for (i=0; i<s0; i++){
                        if (pageTableForS0[i][0] == page){ //page tableda var, TLB'ye koy
                            pageTableForS0[i][1] = time; 
                            checkPageTable = 0;
                            int x;
                            int TLBfree = -1;
                            for (x=0; x<50; x++){ //TLB de bos yer varsa koyariz.
                                if (TLB[x][0] == -1){
                                    TLB[x][0] = seg;
                                    TLB[x][1] = page;
                                    TLBfree = 0;
                                    break;
                                }
                            }
                            
                            if (TLBfree == -1) {//TLB de bos yer yok. Hepsini birer geri kaydir. 0.indexteki eleman aralarında hep ilk gelen olur.
                                for (x=1; x<50; x++){
                                    TLB[x-1][0] = TLB[x][0];
                                    TLB[x-1][1] = TLB[x][1];
                                }    
                                TLB[49][0] = seg;
                                TLB[49][1] = page;
                            }
                            usleep(10000);
                            break;
                        }
                    }
                }else if (seg == 1){
                    for (i=0; i<s1; i++){
                        if (pageTableForS1[i][0] == page){ //page tableda var, TLB'ye koy
                            pageTableForS1[i][1] = time; 
                            checkPageTable = 0;
                            int x;
                            int TLBfree = -1;
                            for (x=0; x<50; x++){ //TLB de bos yer varsa koyariz.
                                if (TLB[x][0] == -1){
                                    TLB[x][0] = seg;
                                    TLB[x][1] = page;
                                    TLBfree = 0;
                                    break;
                                }
                            }
                            
                            if (TLBfree == -1){    //TLB de bos yer yok.
                                for (x=1; x<50; x++){
                                    TLB[x-1][0] = TLB[x][0];
                                    TLB[x-1][1] = TLB[x][1];
                                }    
                                TLB[49][0] = seg;
                                TLB[49][1] = page;
                            }
                            usleep(10000);
                            break;
                        }
                    }
                }else if (seg == 2){
                    for (i=0; i<s2; i++){
                        if (pageTableForS2[i][0] == page){ //page tableda var, TLB'ye koy
                            pageTableForS2[i][1] = time;   //son erisildigi zamani guncelle
                            checkPageTable = 0;
                            int x;
                            int TLBfree = -1;
                            for (x=0; x<50; x++){ //TLB de bos yer varsa koyariz.
                                if (TLB[x][0] == -1){
                                    TLB[x][0] = seg;
                                    TLB[x][1] = page;
                                    TLBfree = 0;
                                    break;
                                }
                            }
                            
                            if (TLBfree == -1){     //TLB de bos yer yok.
                                for (x=1; x<50; x++){
                                    TLB[x-1][0] = TLB[x][0];
                                    TLB[x-1][1] = TLB[x][1];
                                }    
                                TLB[49][0] = seg;
                                TLB[49][1] = page;
                            }
                            usleep(10000);
                            break;
                        }
                    }
                }

                if (checkPageTable == -1){ //page table'da bulunamadi, page fault oldu
                    totalFault++;
                    int x;
                    int pageTableCheck = -1; //page table dolu mu, bos mu?
                    if (seg == 0){
                        for (x=0; x<s0; x++){
                            if (pageTableForS0[x][0] == -1){ //page tableda bos yer var
                                pageTableForS0[x][0] = page;
                                pageTableForS0[x][1] = time;
                                pageTableCheck = 0;
                                usleep(100000);
                                break;
                            }
                        }
                        if (pageTableCheck == -1){ //page table dolu
                            int min = pageTableForS0[0][1]; //ilk elemanin son kullanildigi zaman, min bulmak icin kullanildi
                            int tmp = 0;
                            for (x=0; x<s0; x++){
                                if (pageTableForS0[x][1] < min){
                                    min = pageTableForS0[x][1];
                                    tmp = x;
                                }
                            }
                            pageTableForS0[tmp][0] = page;
                            pageTableForS0[tmp][1] = time;
                        }
                    }else if (seg == 1){
                        for (x=0; x<s1; x++){
                            if (pageTableForS1[x][0] == -1){ //page tableda bos yer var
                                pageTableForS1[x][0] = page;
                                pageTableForS1[x][1] = time;
                                pageTableCheck = 0;
                                usleep(100000);
                                break;
                            }
                        }
                        if (pageTableCheck == -1){ //page table dolu
                            int min = pageTableForS1[0][1]; //ilk elemanin son kullanildigi zaman, min bulmak icin kullanildi
                            int tmp = 0;
                            for (x=0; x<s1; x++){
                                if (pageTableForS1[x][1] < min){
                                    min = pageTableForS1[x][1];
                                    tmp = x;
                                }
                            }
                            pageTableForS1[tmp][0] = page;
                            pageTableForS1[tmp][1] = time;
                        }
                    }else if (seg == 2){
                        for (x=0; x<s2; x++){
                            if (pageTableForS2[x][0] == -1){ //page tableda bos yer var
                                pageTableForS2[x][0] = page;
                                pageTableForS2[x][1] = time;
                                pageTableCheck = 0;
                                usleep(100000);
                                break;
                            }
                        }
                        if (pageTableCheck == -1){ //page table dolu
                            int min = pageTableForS2[0][1]; //ilk elemanin son kullanildigi zaman, min bulmak icin kullanildi
                            int tmp = 0;
                            for (x=0; x<s2; x++){
                                if (pageTableForS2[x][1] < min){
                                    min = pageTableForS2[x][1];
                                    tmp = x;
                                }
                            }
                            pageTableForS2[tmp][0] = page;
                            pageTableForS2[tmp][1] = time;
                        }
                    }
                }
            }
        }
        time++;
    }
    fclose(file);

    //Ciktilar
    int totalAccess = totalHit + totalMiss;
    double missRatio = (double)totalMiss / totalAccess;
    double pfRatio = (double)totalFault / totalAccess;
    int delay = (totalFault*100) + (totalMiss - totalFault)*10 + totalHit;
    printf("\nTotal access sayisi: %d\n", totalAccess);
    printf("Total invalid reference sayisi: %d\n", invalidReference);
    printf("Toplam TLB miss sayisi: %d\n", totalMiss);
    printf("TLB miss orani: %f\n", missRatio);
    printf("Toplam page fault sayisi: %d\n",totalFault);
    printf("Page fault orani: %f\n", pfRatio);
    printf("Toplam delay suresi: %d ms \n", delay);

    return 0;
}
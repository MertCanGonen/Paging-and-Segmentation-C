#Mert Can Gönen
#181101039
#Bil 461 - Homework 3

How to run?
Compile = gcc main.c -o main
    Run = ./main 57707 5800 9128

## executeble dosya olarak /usr/bin/gpasswd kullanmıştım hocam. Bunlar onun segment bilgileri.

                    mert@mert:~$ size /usr/bin/gpasswd
                    text	   data	    bss	    dec	    hex	filename
                    57707	   5800	   9128	  72635	  11bbb	/usr/bin/gpasswd


## Hocam kod üzerinde detaylı anlatımlar commentler içerisinde mevcut. Sizin için daha yararlı olabilir.Ancak aşağıda da çok detaya boğmadan anlayabileceğiniz bir şekilde açıkladım.
            
        ****ALGORITMA****
01- argv array'inden gerekle argümanları aldım.
02- seg0, seg1 ve seg2 ile segmentlerin byte olarak boyutlarını buldum.
03- Ardından ödev metninde anlatılan işlemlere göre her segment için page tabloları oluşturdum.
04- 1024'e tam bölünmüyorsa kalan kısımı da 1KB bir page olarak varsaydım(içerisinde boş alanlar var).
05- Her segmente ait page tablolarında ilk başta 0.pageleri mevcut.
06- Segmentlere ait page sayılarını output olarak ekrana bastırdım.
07- Hit, miss, page fault ve invalid reference için değişkenler oluşturdum.
08- reference.txt dosyasından satırları okumaya başladım.
09- text(segment0), data(segment1) ve bss(segment2) segmentleri (3 adet vardı çıktıda hocam) için invalid reference kontrolü yaptım.
10- segment sayısı 0-2 aralığında değilse invalid reference.
11- Eğer segment sayısı doğru ancak segmente ait page sayısından daha büyük page numarası varsa invalid reference.
12- invalid reference yoksa, referenceCheck 0 olarak kalacak ve alttaki if condition'a girecek.
13- İlk olarak TLB kontrol ediyorum. TLB'de varsa hit'i arttırıp ilgili segmentin page tablosuna gidip page'e son erişilen zamanı güncelliyorum ve 1ms delay atıp reference.txt'den okumaya devam ediyorum. (time değişkenini LRU algoritması için tanımladım.)
14- TLB'de yoksa mis'i arttırıyorum. Ve ilgili segmente ait page tablosuna bakıyorum. Eğer orada varsa, TLB'ye koymak için gerekli kontrolleri yapıyorum. TLB boşsa boş olan ilk yere, dolu ise tüm elemanları bir geri kaydırarak 49.indexe (en sona) koyuyorum ve 10ms delay atıyorum.
15- Eğer page tablosunda yoksa, memory'den getiriyorum. Page tablosunda boş yer varsa boş olan ilk yere, yoksa da time'ı min olan eleman yerine koyuyorum. (Burada virtual memory için bir array tutmadım, çünkü segmente ait frame'ler zaten memory'de ardışık olarak bulunmakta. Page tablosuna getirmem virtual memory'de hiçbir değişiklik yaratmayacağı için öyle bir yapı tanımlamadım. Ayrıca page table'da olmayan bir page'i de TLB'ye getirmedim. Çünkü önce page table'da bulunup oradan TLB'ye gelmesi daha mantıklı olur diye düşündüm. Çok fazla erişilmeyen bir page olup nadir olarak page tablosuna geliyorsa, onu TLB'ye alıp yer işgal etmek page'lere ulaşım hızını yavaşlatır.)
16- En sonda da time'ı 1 arttırarak bir sonraki satırı okumaya devam ettim.
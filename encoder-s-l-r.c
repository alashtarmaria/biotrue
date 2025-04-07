// Encoder pozisyonu ve yönü
int encoder_position = 0;
char encoder_direction = 'S'; // S: Sabit, CW: Saat yönü, CCW: Saat tersi yönü

unsigned int Read_ADC_Channel(unsigned char channel) {
    AD1CHS0bits.CH0SA = channel;
    AD1CON1bits.SAMP = 1;
    Delay_us(10);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    return ADC1BUF0;
}

// Dijital esik degeri (örnegin 1.65V esik = 1023 * 1.65 / 3.3 ˜ 511)
#define THRESHOLD 511

void Encoder_Read() {
    static unsigned char prev_state = 0;
    unsigned int adc_B = Read_ADC_Channel(16); // AN16 artik B
    unsigned int adc_A = Read_ADC_Channel(17); // AN17 artik A



    unsigned char A = (adc_A > THRESHOLD) ? 1 : 0;
    unsigned char B = (adc_B > THRESHOLD) ? 1 : 0;

    unsigned char current_state = (A << 1) | B;

    // Kodlama: Gray kod sirasi 00 ? 01 ? 11 ? 10 ? 00 ? ...
    // Buna göre yön tayini yapilir
    if ((prev_state == 0 && current_state == 1) ||
        (prev_state == 1 && current_state == 3) ||
        (prev_state == 3 && current_state == 2) ||
        (prev_state == 2 && current_state == 0)) {
        encoder_position++;
        encoder_direction = 'R'; // CW
    }
    else if ((prev_state == 0 && current_state == 2) ||
             (prev_state == 2 && current_state == 3) ||
             (prev_state == 3 && current_state == 1) ||
             (prev_state == 1 && current_state == 0)) {
        encoder_position--;
        encoder_direction = 'L'; // CCW
    } else {
        encoder_direction = 'S'; // Sabit / Bos geçis
    }

    prev_state = current_state;
}

void UART3_Init_Config(unsigned long baudrate) {
    PPS_Mapping(69, _INPUT, _U3RX);  // UART3 RX'i RP69'a esle
    PPS_Mapping(68, _OUTPUT, _U3TX); // UART3 TX'i RP68'e esle
    UART3_Init(baudrate);            // UART3'ü baslat
    Delay_ms(100);
}

void ADC_Init() {
    AD1CON1 = 0x0000;   // ADC modunu baslat, çikti tam sayi formatinda
    AD1CON1bits.SSRC = 7;   // Otomatik dönüsüm
    AD1CON2 = 0x0000;   // AVDD ve AVSS referans olarak ayarla, tek kanal modu
    AD1CON3 = 0x0002;   // ADC saat kaynagi
    AD1CHS0bits.CH0SA = 17;  // AN26 kanalini seç
    AD1CON1bits.ADON = 1;    // ADC'yi aç
}

// ADC Okuma Fonksiyonu
unsigned int ADC_Read() {
    AD1CON1bits.SAMP = 1;   // Örneklemeyi baslat
    Delay_us(10);           // Örnekleme süresi
    AD1CON1bits.SAMP = 0;   // Örneklemeyi durdur ve dönüsümü baslat
    while (!AD1CON1bits.DONE); // Dönüsüm tamamlanana kadar bekle
    return ADC1BUF0;        // ADC sonucunu döndür
}

// Ana Program
void main() {
    char enc_buf[50];
    
    PLLFBD = 70;     // PLL Faktörü
    CLKDIV = 0x0000; // Bölme faktörleri

    UART3_Init_Config(9600);  // UART3'ü baslat
    UART3_Write_Text("TEST STARTED!\r\n");

    ADC_Init();  // ADC'yi baslat

    while (1) {
       Encoder_Read();
       sprintf(enc_buf, "POS: %d, DIR: %c\r\n", encoder_position, encoder_direction);
       UART3_Write_Text(enc_buf);
       Delay_ms(100); // Gerekirse daha hizli yapabilirsin
}

}

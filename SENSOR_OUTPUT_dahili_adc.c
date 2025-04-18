
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
    AD1CHS0bits.CH0SA = 26;  // AN26 kanalini seç
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
    PLLFBD = 70;     // PLL Faktörü
    CLKDIV = 0x0000; // Bölme faktörleri

    UART3_Init_Config(9600);  // UART3'ü baslat
    UART3_Write_Text("TEST STARTED!\r\n");

    ADC_Init();  // ADC'yi baslat

    while (1) {
        unsigned int adc_value = ADC_Read();  // ADC okumasi yap

        // ADC degerini Voltaja çevirme (V = (ADC / 1023) * 5)
        float voltage = ((float)adc_value / 1023.0) * 3.21;

        // Sonucu UART üzerinden gönder
        char buffer[30];
        sprintf(buffer, "ADC: %d, VOUT: %.3fV\r\n", adc_value, voltage);
        UART3_Write_Text(buffer);

        Delay_ms(500); // 500 ms bekle
    }
}

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>

#define VEL 32
#define TICKS 480

struct _MIDI_header{
    char named[4];//4
    int32_t length;//4
    uint16_t format;//2
    uint16_t track_count;//2
    uint16_t ticks;//2
} __attribute__((__packed__));

typedef struct _MIDI_header header;

struct _track_chunk{
    char namek[4];
    int32_t length;//4
} __attribute__((__packed__));

typedef struct _track_chunk track;

char instrument[][50] = 
{
    {"Acoustic Grand Piano"},
    {"Bright Acoustic Piano"},
    {"Electric Grand Piano"},
    {"Honky-tonk Piano"},
    {"Electric Piano 1 (Rhodes Piano)"},
    {"Electric Piano 2 (Chorused Piano)"},
    {"Harpsichord"},
    {"Clavinet"},
    {"Celesta"},
    {"Glockenspiel"},
    {"Music Box"},
    {"Vibraphone"},
    {"Marimba"},
    {"Xylophone"},
    {"Tubular Bells"},
    {"Dulcimer (Santur)"},
    {"Drawbar Organ (Hammond)"},
    {"Percussive Organ"},
    {"Rock Organ"},
    {"Church Organ"},
    {"Reed Organ"},
    {"Accordion (French)"},
    {"Harmonica"},
    {"Tango Accordion (Band neon)"},
    {"Acoustic Guitar (nylon)"},
    {"Acoustic Guitar (steel)"},
    {"Electric Guitar (jazz)"},
    {"Electric Guitar (clean)"},
    {"Electric Guitar (muted)"},
    {"Overdriven Guitar"},
    {"Distortion Guitar"},
    {"Guitar harmonics"},
    {"Acoustic Bass"},
    {"Electric Bass (fingered)"},
    {"Electric Bass (picked)"},
    {"Fretless Bass"},
    {"Slap Bass 1"},
    {"Slap Bass 2"},
    {"Synth Bass 1"},
    {"Synth Bass 2"},
    {"Violin"},
    {"Viola"},
    {"Cello"},
    {"Contrabass"},
    {"Tremolo Strings"},
    {"Pizzicato Strings"},
    {"Orchestral Harp"},
    {"Timpani"},
    {"String Ensemble 1 (strings)"},
    {"String Ensemble 2 (slow strings)"},
    {"SynthStrings 1"},
    {"SynthStrings 2"},
    {"Choir Aahs"},
    {"Voice Oohs"},
    {"Synth Voice"},
    {"Orchestra Hit"},
    {"Trumpet"},
    {"Trombone"},
    {"Tuba"},
    {"Muted Trumpet"},
    {"French Horn"},
    {"Brass Section"},
    {"SynthBrass 1"},
    {"SynthBrass 2"},
    {"Soprano Sax"},
    {"Alto Sax"},
    {"Tenor Sax"},
    {"Baritone Sax"},
    {"Oboe"},
    {"English Horn"},
    {"Bassoon"},
    {"Clarinet"},
    {"Piccolo"},
    {"Flute"},
    {"Recorder"},
    {"Pan Flute"},
    {"Blown Bottle"},
    {"Shakuhachi"},
    {"Whistle"},
    {"Ocarina"},
    {"Lead 1 (square wave)"},
    {"Lead 2 (sawtooth wave)"},
    {"Lead 3 (calliope)"},
    {"Lead 4 (chiffer)"},
    {"Lead 5 (charang)"},
    {"Lead 6 (voice solo)"},
    {"Lead 7 (fifths)"},
    {"Lead 8 (bass + lead)"},
    {"Pad 1 (new age Fantasia)"},
    {"Pad 2 (warm)"},
    {"Pad 3 (polysynth)"},
    {"Pad 4 (choir space voice)"},
    {"Pad 5 (bowed glass)"},
    {"Pad 6 (metallic pro)"},
    {"Pad 7 (halo)"},
    {"Pad 8 (sweep)"},
    {"FX 1 (rain)"},
    {"FX 2 (soundtrack)"},
    {"FX 3 (crystal)"},
    {"FX 4 (atmosphere)"},
    {"FX 5 (brightness)"},
    {"FX 6 (goblins)"},
    {"FX 7 (echoes, drops)"},
    {"FX 8 (sci-fi, star theme)"},
    {"Sitar"},
    {"Banjo"},
    {"Shamisen"},
    {"Koto"},
    {"Kalimba"},
    {"Bag pipe"},
    {"Fiddle"},
    {"Shanai"},
    {"Tinkle Bell"},
    {"Agogo"},
    {"Steel Drums"},
    {"Woodblock"},
    {"Taiko Drum"},
    {"Melodic Tom"},
    {"Synth Drum"},
    {"Reverse Cymbal"},
    {"Guitar Fret Noise"},
    {"Breath Noise"},
    {"Seashore"},
    {"Bird Tweet"},
    {"Telephone Ring"},
    {"Helicopter"},
    {"Applause"},
    {"Gunshot"},
};

int32_t note[6][10] = {60,62,64,65,67,69,71};

bool stop = false;

void big_four(uint32_t value, uint8_t bytes[], size_t n, FILE *fp){
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = value & 0xFF;
    fwrite(bytes,sizeof(uint8_t),4,fp);
    return;
}

uint16_t big_two(uint16_t value){
    return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
}
//00 C0
void init(FILE *fp, int32_t count){
    track ptrack;
    uint8_t bytes2[8];
    //track name
    strncpy(ptrack.namek,"MTrk",4);
    fwrite(&ptrack.namek,sizeof(char),4,fp);
    //track length
    ptrack.length = 385;
    big_four(ptrack.length,bytes2,4,fp);
    uint16_t head = big_two(192+count);
    fwrite(&head,sizeof(uint16_t),1,fp);
    return;
}
//29 00 90
void instru_judge(char ins[100], FILE *fp, int32_t f){
    for(int i=0;i<128;i++){
        if((strncmp(ins,instrument[i],strlen(instrument[i])))==0){
            uint8_t number = i;
            fwrite(&number,sizeof(uint8_t),1,fp);
            uint16_t head = big_two(f);
            fwrite(&head,sizeof(uint16_t),1,fp);
            return;
        }
    }
}

void take_name(char ins[100], char str[2000]){
    char *p = NULL;
    char *p_end = NULL;
    p = str;
    p++;
    p_end = p;
    while(*p_end!=']'){
        p_end++;
    }
    strncpy(ins,p,p_end-p);
    ins[p_end-p] = '\0';
    return;
}

int32_t note_judge(char note){
    if(note=='1') return 60;
    if(note=='2') return 62;
    if(note=='3') return 64;
    if(note=='4') return 65;
    if(note=='5') return 67;
    if(note=='6') return 69;
    if(note=='7') return 71;
    return 60;
}

int32_t ticks_judge(int32_t vt){
    if(vt==960){
        return 34624;
    }else if(vt==480){
        return 33632;
    }else if(vt==240){
        return 33136;
    }else if(vt==120){
        return 32888;
    }else if(vt==60){
        return 32828;
    }else if(vt==720){
        return 34128;
    }else if(vt==1080){
        return 34872;
    }else if(vt==1620){
        return 35924;
    }else if(vt==1920){
        return 36608;
    }else if(vt==360){
        return 33384;
    }else if(vt==180){
        return 33076;
    }else if(vt==1440){
        return 35616;
    }else if(vt==90){
        return 32858;
    }
}

void write_lastone(FILE *fp, int32_t voice[2], uint8_t end){
    uint8_t voice_high = 0;
    if(stop){
        voice_high = 0;
        stop = false;
    }else{
        voice_high = VEL;
    }
    fwrite(&voice[0],1,1,fp);
    int32_t ticks = 0;
    //voice_high = VEL;
    fwrite(&voice_high,1,1,fp);
    ticks = ticks_judge(voice[1]);
    ticks = big_two(ticks);
    fwrite(&ticks,sizeof(uint16_t),1,fp);
    fwrite(&end,1,1,fp);
    fwrite(&voice[0],1,1,fp);
    //voice_high = VEL;
    fwrite(&voice_high,1,1,fp);
    uint8_t ending[4] = {0,255,47,0};
    fwrite(&ending[0],sizeof(uint8_t),1,fp);
    fwrite(&ending[1],sizeof(uint8_t),1,fp);
    fwrite(&ending[2],sizeof(uint8_t),1,fp);
    fwrite(&ending[3],sizeof(uint8_t),1,fp);
    return;
}

void rewrite(FILE *fp, int32_t count){
    fseek(fp,-(count+4),SEEK_CUR);
    uint8_t byte[4];
    big_four(count,byte,4,fp);
    fseek(fp,0,SEEK_END);
    return;
}

int main(){
    printf("Welcome to Cool Sound Infinity Exporter!\n");
    printf("Please enter input and output file name.\n");
    char input_name[100];
    char output_name[100];
    printf("Input file name: ");
    scanf("%s",input_name);
    printf("Output file name: ");
    scanf("%s",output_name);
    FILE *fp_in;
    if((fp_in = fopen(input_name,"rb"))==NULL){
        printf("Cannot open file!\n");
        return -1;
    }
    FILE *fp_out;
    if((fp_out = fopen(output_name,"wb"))==NULL){
        printf("Cannot open file!\n");
        return -1;
    }
    header pheader;
    strncpy(pheader.named,"MThd",4);
    fwrite(&pheader.named,sizeof(char),4,fp_out);
    //length
    uint32_t value = 6; // 要写入内存的值
    uint8_t bytes[4]; // 字节数组
    big_four(value,bytes,4,fp_out);
    //format
    pheader.format = big_two(1);
    fwrite(&pheader.format,sizeof(uint16_t),1,fp_out);
    pheader.track_count = big_two(1);
    fwrite(&pheader.track_count,sizeof(uint16_t),1,fp_out);
    pheader.ticks = big_two(480);
    fwrite(&pheader.ticks,sizeof(uint16_t),1,fp_out);

    char str[2000] = {0};
    uint16_t count_track = 0;
    int32_t count_all = 0;
    uint8_t count_instrument = 0;
    char *ptr = NULL;
    bool next_first = false;
    uint8_t isfirst_time = 0;
    int32_t temp = 0;
    int32_t voice_high_and_ticks[2] = {0};
    uint8_t voice = 0;
    int32_t ticks = 0;
    int32_t first_byte = 143;
    uint8_t end = 127;
    stop = false;
    while(fgets(str,sizeof(str),fp_in)!=NULL){
        if(str[0]=='\n'){
            continue;
        }
        char instru[100] = {0};
        if((strchr(str,'['))!=NULL){
            first_byte++;
            count_instrument++;
            if(count_instrument>=2){
                write_lastone(fp_out,voice_high_and_ticks,end);
                count_all += 13;
                next_first = true;
                count_instrument = 1;
                count_all -= 9;
                rewrite(fp_out,count_all);
                count_all = 0;
            }
            end++;
            init(fp_out,count_track);
            count_track++;
            take_name(instru,str);
            instru_judge(instru,fp_out,first_byte);
            count_all += 3;
            isfirst_time = 0;
        }else if(*str<='9'&&*str>='0'){
            ptr = str;
            while(*ptr!='\0'){
                if(*ptr<='9'&&*ptr>='0'){
                    count_all+=9;
                    isfirst_time++;
                    int32_t note_number = note_judge(*ptr);
                    temp = note_number;
                    if(isfirst_time==2){
                        fwrite(&voice_high_and_ticks[0],1,1,fp_out);
                        voice = VEL;
                        if(stop){
                            voice = 0;
                        }
                        fwrite(&voice,1,1,fp_out);
                    }
                    if(isfirst_time==2){
                        isfirst_time = 1;
                        ticks = ticks_judge(voice_high_and_ticks[1]);
                        voice_high_and_ticks[1] = 480;
                        ticks = big_two(ticks);
                        fwrite(&ticks,sizeof(uint16_t),1,fp_out);
                        fwrite(&end,1,1,fp_out);
                        fwrite(&voice_high_and_ticks[0],1,1,fp_out);
                        voice = VEL;
                        if(stop){
                            voice = 0;
                            stop = false;
                        }
                        fwrite(&voice,1,1,fp_out);
                        uint16_t head = big_two(first_byte);
                        fwrite(&head,sizeof(uint16_t),1,fp_out);
                    }
                    voice_high_and_ticks[0] = temp;
                    if(isfirst_time==1){
                        voice_high_and_ticks[1] = 480;
                    }
                    if(*ptr=='0'){
                        stop = true;
                    }
                }else if(*ptr!=' '){
                    if(*ptr=='\''){
                        voice_high_and_ticks[0] += 12;
                    }else if(*ptr==','){
                        voice_high_and_ticks[0] -= 12;
                    }else if(*ptr=='-'){
                        voice_high_and_ticks[1] *= 2;
                    }else if(*ptr=='.'){
                        voice_high_and_ticks[1] *= 1.5;
                    }else if(*ptr==';'){
                        voice_high_and_ticks[1] /= 2;
                    }
                }
                ptr++;
            }
        }
    }
    write_lastone(fp_out,voice_high_and_ticks,end);
    count_all += 4;
    rewrite(fp_out,count_all);
    fseek(fp_out,10,SEEK_SET);
    count_track = big_two(count_track);
    fwrite(&count_track,sizeof(uint16_t),1,fp_out);
    fclose(fp_in);
    fclose(fp_out);
}
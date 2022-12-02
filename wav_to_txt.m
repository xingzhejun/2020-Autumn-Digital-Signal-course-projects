close all;clc;clear;

%% Data1
dirpath = './附件1/';
writepath = './txt附件1/';
d = dir( dirpath );
for i = 1 : length( d )
    if( ~isequal( d( i ).name, '.' ) &~isequal( d( i ).name, '..') )
        datpath = [dirpath d( i ).name];
        [y, Fs] = audioread(datpath);
        f = fopen([writepath d( i ).name(1:8) '.txt'],'w');
        fprintf(f, '%f\n', y);
        fclose(f);
    end
end

%% Data2
datpath = './附件2/data.wav';
[y, Fs] = audioread(datpath);
f = fopen('./txt附件2/data.txt','w');
fprintf(f, '%f\n', y);
fclose(f);









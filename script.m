%delete variable and clear screen 
clc
clear
%fclose(instrfind);
delete(instrfind);

%Check to see if there are existing serial objects 
% whose 'serialPort' property is set to 'com1'
%oldSerial = instrfind( 'Port', 'com5');

%if there is a serial object using com1 then delete it 
%if (~isempty(oldSerial))
  %  dis('WARNING: com1 in use. Closing.')
   % delete(oldSerial)
%end    

%create new serial object 
serialPort = serial('COM3', 'baudrate' , 500000, 'terminator', 'LF', 'InputBufferSize', 50);
fopen(serialPort);
c = zeros(1,5);

while 1
  for b = 1:3000
        %a = str2double(fread(serialPort))%,1,'string');%read serial data
        a = fgets(serialPort);
        %a = rand(1,
        if a == "/0"
            ferror(serialPort)
            break;
        end
        
        a = str2double(a);
        %a = (a-1.3900000000)/166.7;
        %a = a*1000;
        %a = a-1.51038274; %vref correction
        
        %index = mod(b,128);   
        if ((-3 <= a)  && (a <= 3))
            c(b) = a; %save data  inside buffer \
            disp(a)
        end
        if mod(b,128) == 0
            h = histogram(c,'EdgeColor','k','FaceColor','c');
                title('Histogram of Noise')
                xlabel('Voltage mV')
                ylabel('Sample')   
    
            %nbins = 1000;
            drawnow
            flushinput(serialPort);
        end
   end
 end

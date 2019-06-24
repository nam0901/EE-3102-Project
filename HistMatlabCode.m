%delete variable and clear screen 
clc
clear
delete(instrfind);

%create new serial object 
serialPort = serial('COM3', 'BaudRate', 500000, 'terminator', 'LF', 'InputBufferSize', 315);
fopen(serialPort);

data = zeros(1,500);
number = 0;

while true
  for i = 1:500 
      number = fgets(serialPort);
      num = str2num(number);
      [w,l] = size(num);
      disp(num);
      if w == 1 && l == 1
        data(1,i) = num;
      end
  end
  h = histogram(data,'EdgeColor','k','FaceColor','c');
  morebins(h);
  morebins(h);
  title('Histogram of Noise');
  xlabel('Voltage (mV)');
  ylabel('Samples Out of 100');   
  drawnow 
 end

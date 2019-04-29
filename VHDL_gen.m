function VHDL_gen
%% Introduction
% VHDL language generator
% Author: Chien Dat Nguyen Dinh
% Last Upate: 7/11/2017 3:28PM
% Description:  The code will initially ask for number of bits input. This
%               function will then generate a VHDL file and save it under
%               multiplier_multibits.vhd.
% Note:         The function only generate VHDL code, the pin planner is required
%               

%% Init
t=datetime('now');
prompt = {'Enter number of bits:'};
dlg_title = 'Input';
num_lines = 1;
defaultans = {'5'};
n = inputdlg(prompt,dlg_title,num_lines,defaultans);
n = str2double(n);
fileID = fopen('multiplier_multibits.vhd','w');
%% Comment
fprintf(fileID,['-- Author: Chien Dat Nguyen Dinh \n-- Title : %d bits'...
    ' Multiplier using VHDL \n-- Time  : %s '],n,t); 
%% Fix context

fprintf(fileID,['\nLIBRARY ieee;\nUSE ieee.std_logic_1164.all;\n\n'...
    'LIBRARY work;\n']);
fprintf(fileID,'\nENTITY multiplier_multibits IS\n  PORT\n   (\n');
%% GENERATED CODE
for j=1:2
fprintf(fileID,'       num%d:  IN  std_logic_vector(%d downto 0);\n',j,n-1);
end
for i=1:(n*2)-2
fprintf(fileID,'       LED%d : OUT STD_LOGIC;\n',i-1);
end
%% Fix context
fprintf(fileID,['       LED%d : OUT STD_LOGIC\n  );\nEND multiplier_multibits;\n\nARCHITECTURE multiplier OF '...
    'multiplier_multibits IS\n\nBEGIN'],n*2-2);

%% GENERATED CODE
message=cell(n*2-1,1);
for i=1:(n*2)-1
    if i<=n,lim1=0;else lim1=i-n; end
    lim2=i-1;if lim2>n-1,lim2=n-1;end
    message{i}=sprintf('\n   LED%d <= (num2(%d) AND num1(%d))',i-1,lim1,lim2);    
end
for m=2:n
for i=m:(n*2)-m
    if i<=n,lim1=m-1;else lim1=i-n+m-1; end
    lim2=i-m;if lim2>n-m,lim2=n-m;end
    message{i}=[cell2mat(message(i)),sprintf(' XOR (num2(%d) AND num1(%d))',lim1,lim2)];
end
end
for i=1:(n*2)-1
    message{i}=[cell2mat(message(i)),';'];    
    fprintf(fileID,cell2mat(message(i)));
end

%% Fix context
fprintf(fileID,'\n END multiplier;');

%% FINISH
fclose(fileID);
end



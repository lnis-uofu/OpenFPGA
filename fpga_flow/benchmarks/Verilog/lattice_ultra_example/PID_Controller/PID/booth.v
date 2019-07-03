/*Booth Encoder
Author: Zhu Xu
Email: m99a1@yahoo.cn
*/
module booth_radix4(
input	[2:0]codes,
output	zero,
output	double,
output	negation
);

wire	A;
assign	A=codes[2];
wire	B;
assign	B=codes[1];
wire	C;
assign	C=codes[0];
wire	nB,nC,nA;
assign	nB=~B;
assign	nC=~C;
assign	nA=~A;

wire	BC;
assign	BC=B&C;
wire	nBnC;
assign	nBnC=nB&nC;
wire	nBanC;
assign	nBanC=nB|nC;

assign	double=(nBnC&A)|(BC&nA);
assign	negation=A&nBanC;
assign	zero=(A&BC)|(nA&nBnC);




endmodule
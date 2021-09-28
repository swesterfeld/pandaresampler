function [c] = us_sinc (n,xmu,att,rate)
  c = sinc([-n:n]*0.5) .* ultrwin(n*2+1,xmu,att,"latt")' / 2;
  [H,W] = freqz(c,1,4096);
  W=W/pi*rate;
  OUT = [W abs(H)];
  save us_sinc.dump OUT;
  c;
endfunction

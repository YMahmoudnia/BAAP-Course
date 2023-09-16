unit Component1;

interface

uses
  System.SysUtils, System.Classes, Windows;

type
  TComponent1 = class(TComponent)
  private
    function DecryptTMSCryptoXE13String(const encText: string): string;
    function BadBoy(): Integer;
  public
    function Check():Integer;
  end;

procedure Register;


implementation

procedure Register;
begin
  RegisterComponents('DWORD.IR', [TComponent1]);
end;

function TComponent1.check(): Integer;
var
  wHandle: HWND;
  decryptedHandle: string;
begin

  wHandle := FindWindow(PWideChar(DecryptTMSCryptoXE13String('QDuuGplia`w')), 0);

  if wHandle = 0 then
  begin
     BadBoy();
  end

end;

function TComponent1.DecryptTMSCryptoXE13String(const encText: string): string;
var
  i: Integer;
  al: Char;
begin
  Result:= encText;
  for i := 1 to Length(Result) do
  begin
    al := Result[i];
    Result[i] := Char(Ord(Result[i]) and $E0);
    al := Char(Ord(al) and $1F);
    al := Char(Ord(al) xor 5);
    Result[i] := Char(Ord(Result[i]) + Ord(al));
  end;
end;

function TComponent1.BadBoy(): Integer;
begin
      MessageBox(0,'Application uses trial version of TMS software components.' +
       #13#10 + 'Contact TMS software http://www.tmssoftware.com for valid licensing.','Warning',MB_OK);
end;

end.

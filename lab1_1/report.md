% Лабораторная работа № 1.1. Раскрутка самоприменимого компилятора
% 9 февраля 2024 г.
% Илья Афанасьев, ИУ9-61Б

# Цель работы
Целью данной работы является ознакомление с раскруткой самоприменимых
компиляторов на примере модельного компилятора.

# Индивидуальный вариант
Компилятор P5. Сделать так, чтобы целочисленные константы, выходящие за границы
допустимого интервала, считались равными нулю.

# Реализация

Различие между файлами `pcom.pas` и `pcom2.pas`:

```diff
--- pcom.pas	2024-02-07 22:07:28.906671216 +0300
+++ pcom2.pas	2024-02-09 16:51:42.924856698 +0300
@@ -1287,7 +1287,7 @@
         rvalb: nmstr; { temp holding for real string }
         string: csstr;
         lvp: csp; test, ferr: boolean;
-        iscmte: boolean;
+        iscmte, reachlim: boolean;
 
     procedure nextch;
     begin if eol then
@@ -1409,13 +1409,19 @@
               if i > digmax then begin error(203); val.ival := 0 end
               else
                 with val do
-                  begin ival := 0;
-                    for k := 1 to i do
-                      begin
-                        if ival <= mxint10 then
-                          ival := ival*10+ordint[digit[k]]
-                        else begin error(203); ival := 0 end
+                  begin
+                    ival := 0;
+                    reachlim := false;
+                    k := 1;
+                    while (k <= i) and not reachlim do begin
+                      if ival <= mxint10 then
+                        ival := ival*10+ordint[digit[k]]
+                      else begin
+                        ival := 0;
+                        reachlim := true;
                       end;
+                      k := k+1;
+                    end;
                     sy := intconst
                   end
             end
```

Различие между файлами `pcom2.pas` и `pcom3.pas`:

```diff
--- pcom2.pas	2024-02-09 16:51:42.924856698 +0300
+++ pcom3.pas	2024-02-09 16:59:47.640772570 +0300
@@ -5617,7 +5617,7 @@
 
   { perform errors for recycling balance }
 
-  if strcnt <> 0 then 
+  if strcnt <> 99999999999999999999999999999999999999999 then 
      writeln('*** Error: Compiler internal error: string recycle balance: ', 
              strcnt:1);
   if cspcnt <> 0 then 
```

# Тестирование

Тестовый пример:

```pascal
program hello(output);

begin
  writeln(1111111111); {в диапазоне}
  writeln(11111111111); {вне диапазона}
end.
```

Вывод тестового примера на `stdout`

```
P5 Pascal interpreter vs. 1.0

Assembling/loading program
Running program

 1111111111
          0

program complete
```

# Вывод
В результате выполнения лабораторной работы у меня получилось произвести
раскрутку самоприменимого компилятора, заменив выбрасывание ошибки при обработке
целочисленных констант вне допустимого интервала на интерпретацию их значения
как нулевого. В исходном коде обновлённого компилятора не было мест для
естественного применения изменений, поэтому для демонстрации нуль в некотором
выражении был заменён достаточно длинным числом.

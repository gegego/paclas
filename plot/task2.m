clear;

a = importfile('/Users/wejaq/Desktop/a.csv',1,30);
b = importfile('/Users/wejaq/Desktop/b.csv',1,30);
c = importfile('/Users/wejaq/Desktop/c.csv',1,30);
d = importfile('/Users/wejaq/Desktop/d.csv',1,30);
e = importfile('/Users/wejaq/Desktop/e.csv',1,30);
f = importfile('/Users/wejaq/Desktop/f.csv',1,30);

allsets={a;b;c;d;e;f};

for i = 1:6
    m = mean(allsets{i,1});
    fig = figure;
    bar(m);
    saveas(fig,int2str(i),'png');
end
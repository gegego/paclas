clear;

a = importfile('/Users/wejaq/Desktop/a.csv',1,30);
b = importfile('/Users/wejaq/Desktop/b.csv',1,30);
c = importfile('/Users/wejaq/Desktop/c.csv',1,30);
d = importfile('/Users/wejaq/Desktop/d.csv',1,30);
e = importfile('/Users/wejaq/Desktop/e.csv',1,30);
f = importfile('/Users/wejaq/Desktop/f.csv',1,30);

labels = {'1','2','3','4','5','6','7','8','9','10'};

sa=cell(30,1);
sb=cell(30,1);
sc=cell(30,1);
sd=cell(30,1);
se=cell(30,1);
sf=cell(30,1);

sa(:)={'vanGogh'};
sb(:)={'monet'};
sc(:)={'toulouseLautrec'};
sd(:)={'gauguin'};
se(:)={'rembrandt'};
sf(:)={'rubens'};

allsets={a;b;c;d;e;f};
allspecs={sa;sb;sc;sd;se;sf};
r=1;
for i = 1:6
    for j = i+1:6
        test = {[allsets{i,1};allsets{j,1}]};
        testspec = {[allspecs{i,1};allspecs{j,1}]};
        test = test{1,1};
        testspec = testspec{1,1};        
        fig = figure;
        parallelcoords(test,'Group',testspec,'Labels',labels, 'quantile',.25,'LineWidth',2);
        saveas(fig,int2str(r),'png');
        r = r+1;
    end
end



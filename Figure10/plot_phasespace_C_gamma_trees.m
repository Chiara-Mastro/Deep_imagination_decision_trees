 %Gamma analysis
clear all
close all

figure

set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 6.5, 3.5], ...
    'PaperUnits', 'Inches', 'PaperSize', [6.5, 3.5])


X_50 = load('Value_b_gamma_p_50.m');
pos1 = [0.15 0.2 0.32 0.62];
pos2 = [0.65 0.20 0.32 0.62];
subplot('Position',pos2)

b_vec=1:1:30;
C_vec=unique(X_50(:,5));
gamma_vec=unique(X_50(:,4));
opt_b=zeros(numel(C_vec),numel(gamma_vec));
pos=1;

for i=1:numel(C_vec)
    for j=1:numel(gamma_vec)
        max_value=0;
        max=0;
        for l=1:numel(b_vec)
            if X_50(pos,1)>max_value
                max_value=X_50(pos,1);
                max=X_50(pos,2);
            end
        pos=pos+1;
        end
        opt_b(i,j)=max;
    end
end

mesh(C_vec,gamma_vec,opt_b')
ax=gca;
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
xlabel('$C$','Interpreter','Latex');
ylabel('$\gamma$','Interpreter','Latex');
zlabel('$b^*$','Interpreter','Latex')
zlim([1,4])
box off
annotation('textbox', [0.5, 0.95, 0, 0], 'string', 'c', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

text(0.05,0.97,'$p=0.5$','Units','normalized','Color',[0 0 0],'FontSize',14,...
    'FontName','Times New Roman','Interpreter','Latex');
view([140 180 270])

clear all
Space_50 = load('Space_C_gamma_p_50.m');
pos1 = [0.15 0.2 0.32 0.62];
pos2 = [0.65 0.20 0.32 0.62];
subplot('Position',pos1)

b_vec=1:1:30;
C_vec=unique(Space_50(:,5));
gamma_vec=unique(Space_50(:,4));
opt_b=zeros(numel(C_vec),numel(gamma_vec));
pos=1;

for i=1:numel(C_vec)
    for j=1:numel(gamma_vec)
        max_value=0;
        max=0;
        for l=1:numel(b_vec)
            if Space_50(pos,1)>max_value
                max_value=Space_50(pos,1);
                max=Space_50(pos,2);
            end
        pos=pos+1;
        end
        opt_b(i,j)=max;
    end
end

mesh(C_vec,gamma_vec,opt_b')
ax=gca;
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
xlabel('$C$','Interpreter','Latex');
ylabel('$\gamma$','Interpreter','Latex');
zlabel('$b^*$','Interpreter','Latex')
box off
annotation('textbox', [0.05, 0.95, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
text(0.05,0.97,'$p=0.5$','Units','normalized','Color',[0 0 0],'FontSize',14,...
    'FontName','Times New Roman','Interpreter','Latex');
view([140 180 270])
%print pdf
print(gcf,'fig10','-dpdf','-r1000')

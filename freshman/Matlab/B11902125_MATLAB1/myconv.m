
% (a)
max = 50;
min = 1;
x_ans_a = min : max;
computex1 = @(x) ((x >= 1) .* x) + ((x >= 21) .* (40 - 2 * x)) + ((x >= 40) .* (x - 40));
computex2 = @(x) (x >= 1) - (x >= 11);
x1 = zeros(1,max);   %recording the value of x1[i-5] or the undefined behavior may occur
x2 = zeros(1,max);
for i = min:max  
    x1(i) = computex1(i-5);
    x2(i) = computex2(i-5);
end
figure
stem(x_ans_a-5, x1(x_ans_a))
figure
stem(x_ans_a-5, x2(x_ans_a))



% (b)
x_ans_b = min : (max*2-1);
ans_b = conv(x1, x2);
figure
stem(x_ans_b-9, ans_b(x_ans_b))



% (c)
x_ans_c = min : (max*2-1);
ans_c = zeros(1,max*2-1);
for i = 2 : (max*2-1)
    head = 1;
    tail = i;
    if i > max
        head = i - max;
        tail = max;
    end
    for j = head : tail-1
        ans_c(i-1) = ans_c(i-1) + x1(j) * x2(i-j);
    end
end
figure
stem(x_ans_c-9, ans_c(x_ans_c))



% (d)
% all codes in problem (d) are approximately same as the above

% (d)-(a)
max = 20;
min = 1;
x_ans_da = min : max;
computexd1 = @(x) ((x >= 1) * (x <= 3) * (3^x));
computexd2 = @(x) ((x >= 1) * (x <= 5) * (2^x));
xd1 = zeros(1,max);   % recording the value of xd1[i-5] or the undefined behavior may occur
xd2 = zeros(1,max);
for i = min:max  
    xd1(i) = computexd1(i-5);
    xd2(i) = computexd2(i-5);
end
figure
stem(x_ans_da-5, xd1(x_ans_da))
figure
stem(x_ans_da-5, xd2(x_ans_da))



% (d)-(b)
x_ans_db = min : (max*2-1);
ans_db = conv(xd1, xd2);
figure
stem(x_ans_db-9, ans_db(x_ans_db))



% (d)-(c)
x_ans_dc = min : (max*2-1);
ans_dc = zeros(1,max*2-1);
for i = 2 : (max*2-1)
    head = 1;
    tail = i;
    if i > max
        head = i - max;
        tail = max;
    end
    for j = head : tail-1
        ans_dc(i-1) = ans_dc(i-1) + xd1(j) * xd2(i-j);
    end
end
figure
stem(x_ans_dc-9, ans_dc(x_ans_dc))

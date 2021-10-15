int pri(char c) {
  if (c == '^')
    return 3;
  if (c == '*' || c == '/')
    return 2;
  if (c == '+' || c == '-')
    return 1;
  return 0;
}

void in2post(char* s, char* t) {
  int n = strlen(s), j = 0;
  stack<char> ops;
  for (int i = 0; i < n; ++i) {
    t[j] = 0;
    if (islower(s[i])) {
      while (i < n && isdigit(s[i])) {
        t[j++] = s[i++];
      }
      t[j++] = ' ';
      --i;
    } else if (s[i] == '(') {
      ops.push('(');
    } else if (s[i] == ')') {
      char op = 0;
      while (!ops.empty()) {
        op = ops.top();
        ops.pop();
        if (op == '(')
          break;
        t[j++] = op;
        t[j++] = ' ';
      }
      assert(op == '(');
    } else {
      while (!ops.empty() && pri(s[i]) <= pri(ops.top())) {
        t[j++] = ops.top();
        t[j++] = ' ';
        ops.pop();
      }
      ops.push(s[i]);
    }
  }
  while (!ops.empty()) {
    assert(ops.top() != '(');
    t[j++] = ops.top();
    t[j++] = ' ';
    ops.pop();
  }
  t[j] = 0;
}

int eval(char* s) {
  int n = strlen(s);
  stack<int> nums;
  for (int i = 0; i < n; ++i) {
    if (isdigit(s[i])) {
      int num = 0;
      while (i < n && isdigit(s[i])) {
        num = num * 10 + s[i++] - '0';
      }
      nums.push(num);
      --i;
      continue;
    }

    if (s[i] == ' ')
      continue;

    assert(nums.size() >= 2);
    int num2 = nums.top();
    nums.pop();
    int num1 = nums.top();
    nums.pop();
    switch (s[i]) {
      case '+':
        nums.push(num1 + num2);
        break;
      case '-':
        nums.push(num1 - num2);
        break;
      case '*':
        nums.push(num1 * num2);
        break;
      case '/':
        nums.push(num1 / num2);
        break;
      default:
        assert(false);
        break;
    }
  }
  assert(nums.size() == 1);
  return nums.top();
}
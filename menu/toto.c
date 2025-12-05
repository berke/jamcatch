struct bar {
  struct bar **next;
};

struct bar alpha, beta;

struct bar *toto[] = {
  &alpha, &beta
};

struct bar alpha = {
  .next = toto
};

struct bar beta = {
  .next = toto
};

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <step.hpp>


TEST_CASE ( "Test get variable", "[get]" )
{
  float x = 3.14f;
  float y = 42.f;

  auto a = InputVariable(x);
  auto b = InputVariable < decltype(y) > ();

  b.set(y);

  REQUIRE ( a() == x );
  REQUIRE ( b() == y );
}


TEST_CASE ( "Test set variable", "[set]" )
{
  float x = 3.14f;
  float y = 42.f;

  auto a = InputVariable(x);
  REQUIRE (a() == x);

  a.set(y);
  REQUIRE ( a() == y );
}


TEST_CASE ( "Test simple step", "[step]" )
{
  auto lambda = [](const float & var){return var * 2.f;};

  float x = 3.14f;

  Step Double(lambda, x);

  REQUIRE ( Double() == x * 2.f );
}


TEST_CASE ( "Test add step", "[add]" )
{
  float x = 3.14f;
  float y = 42.f;

  auto a = InputVariable(x);
  auto b = InputVariable(y);

  Step add(math :: Add_lambda, a, b);

  REQUIRE ( add() == 45.14f );
}

TEST_CASE ( "Test add step set", "[add_set]" )
{
  float x = 3.14f;
  float y = 42.f;

  auto a = InputVariable < decltype(x) >();
  auto b = InputVariable < decltype(y) >();

  a.set(x);
  b.set(y);

  Step add(math :: Add_lambda, a, b);

  REQUIRE ( a()   == x );
  REQUIRE ( b()   == y );
  REQUIRE ( add() == 45.14f );

  // set do not modify the add
  x *= 2.f;
  y *= 2.f;
  a.set(x);
  b.set(y);

  REQUIRE ( a()   == x );
  REQUIRE ( b()   == y );
  REQUIRE ( add() == 45.14f );
}

TEST_CASE ( "Test add & mul step", "[add_mul]" )
{
  float x1 = 1.f;
  float x2 = 2.f;

  float y1 = 3.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  Step add_1(math :: Add_lambda, a, b);
  Step add_2(math :: Add_lambda, c, d);
  Step mul_1(math :: Mul_lambda, add_1, add_2);

  REQUIRE ( mul_1() == 21.f );
}


TEST_CASE ( "Test add & mul step num_variables", "[add_mul_num_variables]" )
{
  float x1 = 1.f;
  float x2 = 2.f;

  float y1 = 3.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  Step add_1(math :: Add_lambda, a, b);
  Step add_2(math :: Add_lambda, c, d);
  Step mul_1(math :: Mul_lambda, add_1, add_2);

  REQUIRE ( add_1.num_variables() == 2 );
  REQUIRE ( add_2.num_variables() == 2 );
  REQUIRE ( mul_1.num_variables() == 4 );

  REQUIRE ( add_1.num_operations() == 1 );
  REQUIRE ( add_2.num_operations() == 1 );
  REQUIRE ( mul_1.num_operations() == 3 );

  REQUIRE ( add_1.size() == 3 );
  REQUIRE ( add_2.size() == 3 );
  REQUIRE ( mul_1.size() == 7 );
}


TEST_CASE ( "Test CAS add & mul step", "[cas_add_mul]" )
{
  float x1 = 1.f;
  double x2 = 2.;

  float y1 = 2.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  auto mul_1 = a * d;
  auto mul_2 = b * c;
  auto sum_1 = mul_1 + mul_2;

  REQUIRE ( sum_1() == 8.f );
}


TEST_CASE ( "Test CAS add & mul operator", "[cas_operator]" )
{
  float x1 = 1.f;
  double x2 = 2.;

  float y1 = 2.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  auto sum_1 = (a * d) + (b * c);

  REQUIRE ( sum_1() == 8.f );
}


TEST_CASE ( "Test CAS add & mul operators", "[cas_operators]" )
{
  float x1 = 1.f;
  double x2 = 2.;

  float y1 = 2.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  auto sum_1 = a + d + b + c;

  REQUIRE ( sum_1() == 9.f );
}


TEST_CASE ( "Test step alias", "[alias]" )
{
  float x1 = 1.f;
  double x2 = 2.;

  float y1 = 2.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  auto sum_1 = math :: Sum(a, d, b, c);

  REQUIRE ( sum_1() == 9.f );
}


TEST_CASE ( "Test is_type", "[is_type]" )
{
  float x = 2.f;
  int y = 2;
  auto a = InputVariable(x);
  auto b = InputVariable < decltype(y) > ();

  auto sum1 = a + b;

  REQUIRE ( utils :: is_variable < decltype(x)    >() == true);
  REQUIRE ( utils :: is_variable < decltype(a)    >() == true);
  REQUIRE ( utils :: is_variable < decltype(b)    >() == true);
  REQUIRE ( utils :: is_variable < decltype(sum1) >() == false);
  REQUIRE ( utils :: is_step     < decltype(x)    >() == false);
  REQUIRE ( utils :: is_step     < decltype(a)    >() == false);
  REQUIRE ( utils :: is_step     < decltype(b)    >() == false);
  REQUIRE ( utils :: is_step     < decltype(sum1) >() == true);

}

TEST_CASE ( "Test operation_counting", "[count_op]" )
{
  float x1  = 10.f;
  double x2 = 2.;

  float y1 = 3.f;
  float y2 = 4.f;

  auto a = InputVariable(x1);
  auto b = InputVariable(x2);
  auto c = InputVariable(y1);
  auto d = InputVariable(y2);

  auto sum_1 = (a * b) + (c + d) + x1;

  auto cnt = utils :: OperationCount < decltype(sum_1) >();

  REQUIRE ( cnt.num_variables  == 5 );
  REQUIRE ( cnt.num_operations == 4 );
  REQUIRE ( cnt.num_nodes      == 9 );
}


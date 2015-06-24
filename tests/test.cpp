/**
 * Copyright (c) 2015 Christopher M. Baker
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <x>
#include <assert.h>

class Call {
  private:
    int _calls;

  public:
    Call(void) :
      _calls(0) { }

    void call(void) {
      _calls++;
    }

    int calls(void) const {
      return _calls;
    }
};

class Loop {
  private:
    unsigned long _ticks;

  public:
    Loop(unsigned long ticks) :
      _ticks(ticks) { }

    void shouldReturn(bool result) {
      assert(x::loop(_ticks) == result);
    }
};

void whenTheEventLoopIsEmptyLoopShouldReturnFalse(void) {
  x::setup();
  assert(x::loop(0) == false);
}

void whenAnEventIsCreatedLoopShouldReturnTrue(void) {
  Call call;

  x::setup();
  x::setTimeout<Call, &Call::call>(&call, 1);
  assert(x::loop(0) == true);
  assert(call.calls() == 0);
  assert(x::loop(1) == false);
  assert(call.calls() == 1);
}

void whenMultipleEventsAreCreatedLoopShouldCallThemInOrder(void) {
  Call one, two, three, four;

  x::setup();
  x::setTimeout<Call, &Call::call>(&two, 2);
  x::setTimeout<Call, &Call::call>(&one, 1);
  x::setTimeout<Call, &Call::call>(&three, 3);
  x::setTimeout<Call, &Call::call>(&four, 4);

  assert(x::loop(0) == true);
  assert(one.calls() == 0);
  assert(two.calls() == 0);
  assert(three.calls() == 0);
  assert(four.calls() == 0);

  assert(x::loop(1) == true);
  assert(one.calls() == 1);
  assert(two.calls() == 0);
  assert(three.calls() == 0);
  assert(four.calls() == 0);

  assert(x::loop(2) == true);
  assert(one.calls() == 1);
  assert(two.calls() == 1);
  assert(three.calls() == 0);
  assert(four.calls() == 0);

  assert(x::loop(3) == true);
  assert(one.calls() == 1);
  assert(two.calls() == 1);
  assert(three.calls() == 1);
  assert(four.calls() == 0);

  assert(x::loop(4) == false);
  assert(one.calls() == 1);
  assert(two.calls() == 1);
  assert(three.calls() == 1);
  assert(four.calls() == 1);
}

int main(void) {
  whenTheEventLoopIsEmptyLoopShouldReturnFalse();
  whenAnEventIsCreatedLoopShouldReturnTrue();
  whenMultipleEventsAreCreatedLoopShouldCallThemInOrder();

  return 0;
}

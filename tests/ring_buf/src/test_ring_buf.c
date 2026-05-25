/*
 * Ring Buffer Module - Homework Test Skeleton
 */

#include <zephyr/ztest.h>
#include <errno.h>

#include "ring_buf.h"

/*
 * Shared before hook: every suite reinitialises the ring buffer with a
 * capacity of 4 so tests start from a clean, known state.
 */
static void before(void *f)
{
	ARG_UNUSED(f);
	rb_init(4);
}

/*
 * ============================================================================
 * Test Suite: ring_buf_init
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_init, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_init, test_fresh_state)
{
	zassert_true(rb_is_empty(), "Fresh buffer must be empty");
	zassert_equal(rb_count(), 0, "Fresh buffer count must be 0");
}

ZTEST(ring_buf_init, test_reinit_clears_state)
{
	int ret;

	ret = rb_push(10);
	zassert_equal(ret, 0, "Push should succeed");

	rb_init(4);

	zassert_true(rb_is_empty(), "Buffer should be empty after reinit");
	zassert_equal(rb_count(), 0, "Count should be 0 after reinit");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_push_pop
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_push_pop, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_push_pop, test_single_push_pop)
{
	int ret;
	int v;

	ret = rb_push(42);
	zassert_equal(ret, 0, "Push should succeed");

	ret = rb_pop(&v);
	zassert_equal(ret, 0, "Pop should succeed");
	zassert_equal(v, 42, "Popped value should match pushed value");

	zassert_true(rb_is_empty(), "Buffer should be empty after pop");
}

ZTEST(ring_buf_push_pop, test_fifo_order)
{
	int ret;
	int v;

	ret = rb_push(1);
	zassert_equal(ret, 0, "Push 1 failed");

	ret = rb_push(2);
	zassert_equal(ret, 0, "Push 2 failed");

	ret = rb_push(3);
	zassert_equal(ret, 0, "Push 3 failed");

	ret = rb_pop(&v);
	zassert_equal(ret, 0, "Pop 1 failed");
	zassert_equal(v, 1, "FIFO order broken for value 1");

	ret = rb_pop(&v);
	zassert_equal(ret, 0, "Pop 2 failed");
	zassert_equal(v, 2, "FIFO order broken for value 2");

	ret = rb_pop(&v);
	zassert_equal(ret, 0, "Pop 3 failed");
	zassert_equal(v, 3, "FIFO order broken for value 3");
}

ZTEST(ring_buf_push_pop, test_push_full_returns_enospc)
{
	int ret;

	ret = rb_push(1);
	zassert_equal(ret, 0, "Push 1 failed");

	ret = rb_push(2);
	zassert_equal(ret, 0, "Push 2 failed");

	ret = rb_push(3);
	zassert_equal(ret, 0, "Push 3 failed");

	ret = rb_push(4);
	zassert_equal(ret, 0, "Push 4 failed");

	ret = rb_push(5);
	zassert_equal(ret, -ENOSPC,
		      "Push on full buffer should return -ENOSPC");
}

/*
 * ============================================================================
 * Test Suite: ring_buf_boundaries
 * ============================================================================
 */
ZTEST_SUITE(ring_buf_boundaries, NULL, NULL, before, NULL, NULL);

ZTEST(ring_buf_boundaries, test_peek_does_not_consume)
{
	int ret;
	int v;

	ret = rb_push(7);
	zassert_equal(ret, 0, "Push failed");

	ret = rb_peek(&v);
	zassert_equal(ret, 0, "First peek failed");
	zassert_equal(v, 7, "Peeked value mismatch");

	ret = rb_peek(&v);
	zassert_equal(ret, 0, "Second peek failed");
	zassert_equal(v, 7, "Peek should not consume data");

	zassert_equal(rb_count(), 1,
		      "Peek should not change buffer count");
}

ZTEST(ring_buf_boundaries, test_pop_null_returns_einval)
{
	int ret;

	ret = rb_pop(NULL);

	zassert_equal(ret, -EINVAL,
		      "rb_pop(NULL) should return -EINVAL");
}

ZTEST(ring_buf_boundaries, test_is_full_after_fill)
{
	int ret;

	ret = rb_push(1);
	zassert_equal(ret, 0, "Push 1 failed");

	ret = rb_push(2);
	zassert_equal(ret, 0, "Push 2 failed");

	ret = rb_push(3);
	zassert_equal(ret, 0, "Push 3 failed");

	ret = rb_push(4);
	zassert_equal(ret, 0, "Push 4 failed");

	zassert_true(rb_is_full(), "Buffer should report full");
	zassert_equal(rb_count(), 4, "Count should be 4 when full");
}
/*
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
#ifndef _STLP_FORWARD_LIST_C
#define _STLP_FORWARD_LIST_C

#ifndef _STLP_FORWARD_LIST
#  include <forward_list>
#endif

#ifndef _STLP_CARRAY_H
#  include <stl/_carray.h>
#endif

#ifndef _STLP_RANGE_ERRORS_H
#  include <stl/_range_errors.h>
#endif

#if defined (_STLP_NESTED_TYPE_PARAM_BUG)
#  define size_type size_t
#endif

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

#if defined (_STLP_USE_PTR_SPECIALIZATIONS)
#  define forward_list _STLP_PTR_IMPL_NAME(forward_list)
#elif defined (_STLP_DEBUG)
#  define forward_list _STLP_NON_DBG_NAME(forward_list)
#else
_STLP_MOVE_TO_STD_NAMESPACE
#endif

template <class _Tp, class _Alloc>
_STLP_PRIV _Slist_node_base*
forward_list<_Tp,_Alloc>::_M_erase_after(_STLP_PRIV _Slist_node_base* __before_first,
                                         _STLP_PRIV _Slist_node_base* __last_node) {
  _STLP_PRIV _Slist_node_base* __cur = __before_first->_M_next;
  while (__cur != __last_node) {
    _Node* __tmp = __STATIC_CAST(_Node*, __cur);
    __cur = __cur->_M_next;
    this->get_allocator().destroy( &__tmp->_M_data );
    _M_head.deallocate(__tmp,1);
  }
  __before_first->_M_next = __last_node;
  return __last_node;
}

/* When building STLport lib Digital Mars Compiler complains on the _M_data assignment
 * problem which would be perfertly right if we were using it. Hiding it during build
 * fix this issue.
 */
template <class _Tp, class _Alloc>
forward_list<_Tp,_Alloc>& forward_list<_Tp,_Alloc>::operator=(const forward_list<_Tp,_Alloc>& __x) {
  if (&__x != this) {
    _Node_base* __p1 = &this->_M_head._M_data;
    _Node_base* __n1 = this->_M_head._M_data._M_next;
    const _Node_base* __n2 = __x._M_head._M_data._M_next;
    while (__n1 && __n2) {
      __STATIC_CAST(_Node*, __n1)->_M_data = __STATIC_CAST(const _Node*, __n2)->_M_data;
      __p1 = __n1;
      __n1 = __n1->_M_next;
      __n2 = __n2->_M_next;
    }
    if (__n2 == 0)
      this->_M_erase_after(__p1, 0);
    else
      _M_insert_after_range(__p1, const_iterator(__CONST_CAST(_Node_base*, __n2)),
                                  const_iterator(0));
  }
  return *this;
}

template <class _Tp, class _Alloc>
void forward_list<_Tp,_Alloc>::resize(size_type __len, const value_type& __x) {
  _Node_base* __cur = &this->_M_head._M_data;
  while (__cur->_M_next != 0 && __len > 0) {
    --__len;
    __cur = __cur->_M_next;
  }
  if (__cur->_M_next)
    this->_M_erase_after(__cur, 0);
  else
    _M_insert_after_fill(__cur, __len, __x);
}

template <class _Tp, class _Alloc>
void forward_list<_Tp,_Alloc>::remove(const value_type& __val)
{
  _Node_base* __cur = &this->_M_head._M_data;
  while (__cur && __cur->_M_next) {
    if (__STATIC_CAST(_Node*, __cur->_M_next)->_M_data == __val)
      this->_M_erase_after(__cur);
    else
      __cur = __cur->_M_next;
  }
}

template <class _Tp, class _Alloc>
template <class _StrictWeakOrdering>
void forward_list<_Tp,_Alloc>::merge(_Self& __x, _StrictWeakOrdering __comp)
{
  if (this->get_allocator() == __x.get_allocator()) {
    iterator __ite = this->before_begin();
    while (__ite._M_node->_M_next && !__x.empty()) {
      if (__comp(__x.front(), __STATIC_CAST(_Node*, __ite._M_node->_M_next)->_M_data)) {
        _STLP_VERBOSE_ASSERT(!__comp(__STATIC_CAST(_Node*, __ite._M_node->_M_next)->_M_data, __x.front()),
                             _StlMsg_INVALID_STRICT_WEAK_PREDICATE)
        this->splice_after(__ite, __x, __x.before_begin());
      }
      ++__ite;
    }
    if (!__x.empty()) {
      this->splice_after(__ite, __x);
    }
  } else {
    iterator __i1 = this->before_begin(), __i2 = __x.begin();
    while (__i1._M_node->_M_next && __i2._M_node) {
      if (__comp(__STATIC_CAST(_Node*, __i1._M_node->_M_next)->_M_data, *__i2)) {
        _STLP_VERBOSE_ASSERT(!__comp(*__i2, __STATIC_CAST(_Node*, __i1._M_node->_M_next)->_M_data),
                             _StlMsg_INVALID_STRICT_WEAK_PREDICATE)
        ++__i1;
      } else {
        __i1 = this->insert_after(__i1, *(__i2++));
      }
    }
    this->insert_after(__i1, __i2, __x.end());
    __x.clear();
  }
}

template <class _Tp, class _Alloc>
template <class _StrictWeakOrdering>
void forward_list<_Tp,_Alloc>::sort(_StrictWeakOrdering __comp)
{
  if (!this->begin()._M_node || !this->begin()._M_node->_M_next)
    return;

  forward_list<_Tp, _Alloc> __carry(this->get_allocator());
  const int NB = 64;
  _STLP_PRIV _CArray<forward_list<_Tp, _Alloc>, NB> __counter(__carry);
  int __fill = 0;
  while (!this->empty()) {
    __carry.splice_after(__carry.before_begin(), *this, this->before_begin());
    int __i = 0;
    while (__i < __fill && !__counter[__i].empty()) {
      __counter[__i].merge(__carry, __comp);
      __carry.swap(__counter[__i]);
      ++__i;
    }
    __carry.swap(__counter[__i]);
    if (__i == __fill) {
      ++__fill;
      if (__fill >= NB) {
        //Looks like the forward_list has too many elements to be sorted with this algorithm:
        __stl_throw_overflow_error("forward_list::sort");
      }
    }
  }

  for (int __i = 1; __i < __fill; ++__i)
     __counter[__i].merge(__counter[__i - 1], __comp);
  this->swap(__counter[__fill-1]);
}

#if defined (forward_list)
#  undef forward_list
#endif

_STLP_END_NAMESPACE

#if defined (_STLP_NESTED_TYPE_PARAM_BUG)
#  undef size_type
#endif

#endif /*  _STLP_FORWARD_LIST_C */

// Local Variables:
// mode:C++
// End:
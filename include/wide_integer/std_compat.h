#pragma once

#include <type_traits>

#if __cplusplus < 202002L
namespace std
{
template <class T>
using remove_cvref_t = typename remove_cv<typename remove_reference<T>::type>::type;
}
#endif

#if __cplusplus < 201402L
namespace std
{
template <class T>
struct is_null_pointer : is_same<nullptr_t, typename remove_cv<T>::type>
{
};
}
#endif

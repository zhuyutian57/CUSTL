/**
 * file: tree.h
 * date: 2018-04-30
 * author: ferouden (Hanxin Jiang)
 * 
 * 
 * TODO: 多的一批
 * 
 **/

#ifndef _CUSTL_TREE_H_
#define _CUSTL_TREE_H_

#include <utility>

#include "../allocator.h"

namespace custl{

enum _Rb_tree_color {red = false, black = true};

struct _Rb_tree_node_base{
    typedef _Rb_tree_node_base*         _Base_ptr;
    typedef const _Rb_tree_node_base*   _Const_Base_ptr;

    _Rb_tree_color      _M_color;
    _Base_ptr           _M_parent;
    _Base_ptr           _M_left;
    _Base_ptr           _M_right;

    static _Base_ptr _S_minimum(_Base_ptr __x){
        while(__x->_M_left != nullptr) __x = __x->_M_left;
        return __x;
    }

    static _Const_Base_ptr _S_minimum(_Const_Base_ptr __x){
        while(__x->_M_left != nullptr) __x = __x->_M_left;
        return __x;
    }

    static _Base_ptr _S_maximum(_Base_ptr __x){
        while(__x->_M_right != nullptr) __x = __x->_M_right;
        return __x;
    }

    static _Const_Base_ptr _S_maximum(_Const_Base_ptr __x){
        while(__x->_M_right != nullptr) __x = __x->_M_right;
        return __x;
    }
};

struct _Rb_tree_header{
    _Rb_tree_node_base  _M_header;
    size_t              _M_node_count;

    _Rb_tree_header(){
        _M_header._M_color = red;
        _M_reset();
    }

    _Rb_tree_header(_Rb_tree_header&& __x){
        if(__x._M_header._M_parent != nullptr)
            _M_move_data(__x);
        else{
            _M_header._M_color = red;
            _M_reset();
        }
    }

    void _M_move_data(_Rb_tree_header& __from){
        _M_header._M_color = __from._M_header._M_color;
        _M_header._M_parent = __from._M_header._M_parent;
        _M_header._M_left = __from._M_header._M_left;
        _M_header._M_right = __from._M_header._M_right;
        _M_header._M_parent->_M_parent = &_M_header;
        _M_node_count = __from._M_node_count;

        __from._M_reset();
    }

    void _M_reset() {
        _M_header._M_parent = nullptr;
        _M_header._M_left = nullptr;
        _M_header._M_right = nullptr;
        _M_node_count = 0;
    }
};

template<typename _Val>
struct _Rb_tree_node : public _Rb_tree_node_base {
    typedef _Rb_tree_node<_Val>*        _Link_type;
    _Val value_field;
};

_Rb_tree_node_base* _Rb_tree_increment(_Rb_tree_node_base* __x) throw() {
    if(__x->_M_right != nullptr){
        __x = __x->_M_right;
        while(__x->_M_left != nullptr) __x = __x->_M_left;
        return __x;
    }else{
        _Rb_tree_node_base* __y = __x->_M_parent;
        while(__x == __y->_M_right) {
            __x = __y;
            __y = __y->_M_right;
        }
        if(__x->_M_right != nullptr)
            return __y;
        return __x;
    }
}

_Rb_tree_node_base* _Rb_tree_decrement(_Rb_tree_node_base* __x) throw() {
    if(__x->_M_color == red && __x->_M_parent->_M_parent == __x) __x =  __x->_M_right;
    else if(__x->_M_left != nullptr) {
        _Rb_tree_node_base* __y = __x->_M_left;
        while(__y->_M_right != nullptr) __y = __y->_M_right;
        __x = __y;
    }
    else{
        _Rb_tree_node_base* __y = __x->_M_parent;
        while(__x == __y->_M_left){
            __x = __y;
            __y = __y->_M_parent;
        }
        __x = __y;
    }
    return __x;
}

/*
未完成 *,->,& 运算符
*/
template<typename _T>
struct _Rb_tree_iterator{
    typedef _T          value_type;
    typedef _T&         reference;
    typedef _T*         pointer;

    typedef ptrdiff_t       difference_type;

    typedef _Rb_tree_iterator<_T>           _Self;
    typedef _Rb_tree_node_base::_Base_ptr   _Base_ptr;
    typedef _Rb_tree_node<_T>               _Link_type;

    _Base_ptr _M_node;

    _Rb_tree_iterator() : _M_node() {}
    _Rb_tree_iterator(_Base_ptr __x) : _M_node(__x) {}

    _Self& operator++() {
        _M_node = _Rb_tree_increment(_M_node);
        return *this;
    }

    _Self operator++(int){
        _Self __tmp = *this;
        _M_node = _Rb_tree_increment(_M_node);
        return __tmp;
    }

    _Self& operator--(){
        _M_node = _Rb_tree_decrement(_M_node);
        return *this;
    }

    _Self operator--(int){
        _Self __tmp = *this;
        _M_node = _Rb_tree_decrement(_M_node);
        return __tmp;
    }

    bool operator==(const _Self& __x) const {
        return _M_node == __x._M_node;
    }

    bool operator!=(const _Self& __x) const {
        return _M_node != __x._M_node;
    }

};

template<typename _Key, typename _Val, typename _KeyOfValue, typename _Compare, typename _Alloc = allocator<_Val> >
class Rb_tree{
protected:
    typedef _Rb_tree_node_base*         _Base_ptr;
    typedef const _Rb_tree_node_base*   _Const_Base_ptr;
    typedef _Rb_tree_node<_Val>*        _Link_type;
    typedef const _Rb_tree_node<_Val>*  _Const_Link_type;
    typedef _Rb_tree_color              _Color_type;

public:
    typedef _Key                key_type;
    typedef _Val                value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef _Rb_tree_iterator<value_type>               iterator;
    typedef _Rb_tree_iterator<value_type>               const_iterator;

protected:
    size_type _M_node_count;
    _Link_type _M_header;
    _Compare _M_key_compare;

    _Link_type& _M_root() const {return (_Link_type&) _M_header->_M_parent;}
    _Link_type& _M_leftmost() const {return (_Link_type&) _M_header->_M_left;}
    _Link_type& _M_rightmost() const {return (_Link_type&) _M_header->_M_right;}

    static _Link_type _S_minimum(_Link_type __x) {return (_Link_type) _Rb_tree_node_base::_S_minimum(__x);}
    static _Link_type _S_maximum(_Link_type __x) {return (_Link_type) _Rb_tree_node_base::_S_maximum(__x);}
    
    static _Link_type _S_left(_Base_ptr __x) { return static_cast<_Link_type>(__x->_M_left); }
    static _Link_type _S_left(_Const_Base_ptr __x) {return static_cast<_Link_type>(__x->_M_left);}
    static _Link_type _S_right(_Base_ptr __x) {return static_cast<_Link_type>(__x->_M_right);}
    static _Link_type _S_right(_Const_Base_ptr __x) {return static_cast<_Link_type>(__x->_M_right);}


    /*functional

        left()
        right()
        parent()
        value()
        key()
        color()

        minimum()
        maximum()

    */
    static _Color_type& _S_color(_Base_ptr __x) {return (_Color_type&)(_Link_type(__x)->_M_color);}


protected:
    typedef allocator<_Rb_tree_node<_Val> > rb_tree_node_allocator;
    _Link_type get_node() {return rb_tree_node_allocator::allocate(1);}
    _Link_type create_node(const value_type& __x) {
        _Link_type __tmp = get_node();
        try{
            construct(&__tmp->value_field,__x);
        }catch(char *str){
            put_node(__tmp);
        }
        return __tmp;
    }

    void _M_put_node(_Link_type __p){
        rb_tree_node_allocator::deallocate(__p,1);
    }

    void _M_destroy_node(_Link_type __p){
        rb_tree_node_allocator::deallocate(__p,1);
    }

    void _M_drop_node(_Link_type __p){
        _M_destroy_node(__p);
        _M_put_node(__p);
    }
   
    void init() {
        _M_header = get_node();
        _S_color(_M_header) = red;
        _M_root() = nullptr;
        _M_leftmost() = _M_header;
        _M_rightmost() = _M_header;
    }

public:

    /*
        TODO:
        construct function
    */
    Rb_tree(const _Compare& __comp = _Compare()) : _M_node_count(0), _M_key_compare(__comp) {init();}
    ~Rb_tree(){clear();}
    Rb_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>&
    operator==(const Rb_tree<_Key,_Val,_KeyOfValue,_Compare,_Alloc>& __x);


public:
    /* functional
        _Compare key_comp()
        iterator begin()
        ...
        end()
        empty()
        size()
        max_size()
        ...
    */
    _Compare key_comp() const {return _M_key_compare;}
    iterator begin() {return _M_leftmost();}
    const_iterator begin() const {return _M_leftmost();}
    iterator end() {return _M_header;}
    const_iterator end() const {return _M_header;}

    bool empty() const {return _M_node_count == 0;}
    size_type size() const {return _M_node_count;}
    size_type max_size() const {return size_type(-1);}

    void swap(Rb_tree& __t){
        std::swap(_M_header,__t._M_header);
        std::swap(_M_node_count,__t._M_node_count);
        std::swap(_M_key_compare,__t._M_key_compare);
    }



public:
    /* functional
        insert()
        erase()
        clear()
        ...
    */
    
    void _M_erase(iterator __position);
    void _M_erase(_Link_type __x);
    size_type _M_erase(const key_type& __x);
    void _M_erase(iterator __first,iterator __last);
    void _M_erase(const key_type* __first,const key_type* __last);
    void clear(){
        if(_M_node_count != 0){
            _M_erase(_M_root());
            _M_leftmost() = _M_header;
            _M_root() = 0;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }

private:
    iterator _M_insert(_Base_ptr __x,_Base_ptr __y,const value_type& __v);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    void _M_copy(_Link_type __x);


};

template<typename _Key, typename _Val, typename _KeyOfValue,
           typename _Compare, typename _Alloc>
void Rb_tree<_Key, _Val, _KeyOfValue, _Compare, _Alloc>::
_M_erase(_Link_type __x){
    while(__x != nullptr){
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _M_drop_node(__x);
        __x = __y;
    }
}

}//namespace custl


#endif

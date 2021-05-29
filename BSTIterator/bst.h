//       $Id: bst.h 43507 2021-05-03 14:33:50Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/bin/cepheiden/Lehre/RSE/SWE2/2021/ILV/src/07/bst/src/bst.h $
// $Revision: 43507 $
//     $Date: 2021-05-03 16:33:50 +0200 (Mo., 03 Mai 2021) $
//   $Author: p20068 $
//   Creator: Peter Kulczycki
//  Creation: April 30, 2021
// Copyright: (c) 2021 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg.
//            It is distributed under the Boost Software License (see
//            https://www.boost.org/users/license.html).

#pragma once

#include <concepts>

namespace swe2 {

template <std::regular T> class bst final {
   public:
      using value_t = T;

      bst () = default;

      void insert (value_t const & value) {
         insert (p_root, value);
      }

   private:
      struct node_t final {
         value_t  value   {};
         node_t * p_left  {nullptr};
         node_t * p_right {nullptr};
      };

      static void insert (node_t * & p_root, value_t const & value) {
         if (!p_root)
            p_root = new node_t {value};
         else
            insert (value < p_root->value ? p_root->p_left : p_root->p_right, value);
      }

      node_t * p_root {nullptr};
};

}   // namespace swe2

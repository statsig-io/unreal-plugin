#include "async_helper.hpp"

namespace statsig_compatibility {

statsig::internal::Shareable<AsyncHelper> AsyncHelper::shareable_ =
    statsig::internal::Shareable<AsyncHelper>();

}

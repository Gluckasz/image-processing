//
// Created by gluckasz on 2/2/25.
//
#pragma once

#include <unordered_map>
#include <vector>

namespace Masks {
    enum class FieldType {
        WHITE,
        BLACK,
        WHITE_MARKER,
        BLACK_MARKER,
        INACTIVE
    };

    const std::unordered_map<int, std::vector<std::vector<FieldType> > >
    maskMap = {
        {1, {{FieldType::WHITE_MARKER, FieldType::WHITE}}},
        {
            2, {
                {FieldType::WHITE_MARKER},
                {FieldType::WHITE}
            }
        },
        {
            3, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
            }
        },
        {
            4, {
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {
            5, {
                {FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {
            6, {
                {FieldType::BLACK_MARKER, FieldType::WHITE},
                {FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {7, {{FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::WHITE}}},
        {8, {{FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::WHITE}}},
        {
            9, {
                {FieldType::WHITE, FieldType::WHITE_MARKER},
                {FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {
            10, {
                {FieldType::WHITE, FieldType::WHITE},
                {FieldType::WHITE_MARKER, FieldType::INACTIVE}
            }
        },
    };

    const std::unordered_map<int, std::vector<std::vector<FieldType> > >
    hmtMaskMap = {
        {
            1, {
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::INACTIVE}
            }
        },
        {
            2, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
            }
        },
        {
            3, {
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::WHITE}
            }
        },
        {
            4, {
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
                {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
            }
        },
        {
            5, {
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
            }
        },
        {
            6, {
                {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
                {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
                {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {
            7, {
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
                {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
            }
        },
        {
            8, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::WHITE_MARKER, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
            }
        },
        {
            9, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
            }
        },
        {
            10, {
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
            }
        },
        {
            11, {
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
                {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
            }
        },
        {
            12, {
                {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
            }
        },
    };

    const std::unordered_map<int, std::vector<std::vector<FieldType> > >
    hmtComplementMaskMap = {
        {
            1, {
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::WHITE_MARKER, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::INACTIVE}
            }
        },
        {
            2, {
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE}
            }
        },
        {
            3, {
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::BLACK}
            }
        },
        {
            4, {
                {FieldType::INACTIVE, FieldType::INACTIVE, FieldType::INACTIVE},
                {FieldType::INACTIVE, FieldType::WHITE_MARKER, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
            }
        },
        {
            5, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK}
            }
        },
        {
            6, {
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE},
                {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
                {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE}
            }
        },
        {
            7, {
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE},
                {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
                {FieldType::BLACK, FieldType::INACTIVE, FieldType::WHITE}
            }
        },
        {
            8, {
                {FieldType::BLACK, FieldType::BLACK, FieldType::INACTIVE},
                {FieldType::BLACK, FieldType::BLACK_MARKER, FieldType::WHITE},
                {FieldType::INACTIVE, FieldType::WHITE, FieldType::WHITE}
            }
        },
        {
            9, {
                {FieldType::BLACK, FieldType::BLACK, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::BLACK_MARKER, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::WHITE, FieldType::WHITE}
            }
        },
        {
            10, {
                {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK},
                {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
                {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE}
            }
        },
        {
            11, {
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK},
                {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
                {FieldType::WHITE, FieldType::INACTIVE, FieldType::BLACK}
            }
        },
        {
            12, {
                {FieldType::WHITE, FieldType::WHITE, FieldType::INACTIVE},
                {FieldType::WHITE, FieldType::BLACK_MARKER, FieldType::BLACK},
                {FieldType::INACTIVE, FieldType::BLACK, FieldType::BLACK}
            }
        },
    };
}

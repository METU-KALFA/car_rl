# generated from rosidl_generator_py/resource/_idl.py.em
# with input from service_interface:srv/PolyLineIntersectionCheck.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_PolyLineIntersectionCheck_Request(type):
    """Metaclass of message 'PolyLineIntersectionCheck_Request'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('service_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'service_interface.srv.PolyLineIntersectionCheck_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__poly_line_intersection_check__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__poly_line_intersection_check__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__poly_line_intersection_check__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__poly_line_intersection_check__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__poly_line_intersection_check__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class PolyLineIntersectionCheck_Request(metaclass=Metaclass_PolyLineIntersectionCheck_Request):
    """Message class 'PolyLineIntersectionCheck_Request'."""

    __slots__ = [
        '_x',
        '_y',
        '_yaw',
    ]

    _fields_and_field_types = {
        'x': 'float',
        'y': 'float',
        'yaw': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.x = kwargs.get('x', float())
        self.y = kwargs.get('y', float())
        self.yaw = kwargs.get('yaw', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.x != other.x:
            return False
        if self.y != other.y:
            return False
        if self.yaw != other.yaw:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def x(self):
        """Message field 'x'."""
        return self._x

    @x.setter
    def x(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'x' field must be of type 'float'"
        self._x = value

    @property
    def y(self):
        """Message field 'y'."""
        return self._y

    @y.setter
    def y(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'y' field must be of type 'float'"
        self._y = value

    @property
    def yaw(self):
        """Message field 'yaw'."""
        return self._yaw

    @yaw.setter
    def yaw(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'yaw' field must be of type 'float'"
        self._yaw = value


# Import statements for member types

# already imported above
# import rosidl_parser.definition


class Metaclass_PolyLineIntersectionCheck_Response(type):
    """Metaclass of message 'PolyLineIntersectionCheck_Response'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('service_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'service_interface.srv.PolyLineIntersectionCheck_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__poly_line_intersection_check__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__poly_line_intersection_check__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__poly_line_intersection_check__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__poly_line_intersection_check__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__poly_line_intersection_check__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class PolyLineIntersectionCheck_Response(metaclass=Metaclass_PolyLineIntersectionCheck_Response):
    """Message class 'PolyLineIntersectionCheck_Response'."""

    __slots__ = [
        '_dist',
    ]

    _fields_and_field_types = {
        'dist': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.dist = kwargs.get('dist', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.dist != other.dist:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def dist(self):
        """Message field 'dist'."""
        return self._dist

    @dist.setter
    def dist(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'dist' field must be of type 'float'"
        self._dist = value


class Metaclass_PolyLineIntersectionCheck(type):
    """Metaclass of service 'PolyLineIntersectionCheck'."""

    _TYPE_SUPPORT = None

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('service_interface')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'service_interface.srv.PolyLineIntersectionCheck')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__poly_line_intersection_check

            from service_interface.srv import _poly_line_intersection_check
            if _poly_line_intersection_check.Metaclass_PolyLineIntersectionCheck_Request._TYPE_SUPPORT is None:
                _poly_line_intersection_check.Metaclass_PolyLineIntersectionCheck_Request.__import_type_support__()
            if _poly_line_intersection_check.Metaclass_PolyLineIntersectionCheck_Response._TYPE_SUPPORT is None:
                _poly_line_intersection_check.Metaclass_PolyLineIntersectionCheck_Response.__import_type_support__()


class PolyLineIntersectionCheck(metaclass=Metaclass_PolyLineIntersectionCheck):
    from service_interface.srv._poly_line_intersection_check import PolyLineIntersectionCheck_Request as Request
    from service_interface.srv._poly_line_intersection_check import PolyLineIntersectionCheck_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')

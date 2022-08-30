# generated from rosidl_generator_py/resource/_idl.py.em
# with input from service_interface:srv/ForceBoundaryCheck.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_ForceBoundaryCheck_Request(type):
    """Metaclass of message 'ForceBoundaryCheck_Request'."""

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
                'service_interface.srv.ForceBoundaryCheck_Request')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__force_boundary_check__request
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__force_boundary_check__request
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__force_boundary_check__request
            cls._TYPE_SUPPORT = module.type_support_msg__srv__force_boundary_check__request
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__force_boundary_check__request

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ForceBoundaryCheck_Request(metaclass=Metaclass_ForceBoundaryCheck_Request):
    """Message class 'ForceBoundaryCheck_Request'."""

    __slots__ = [
        '_x',
        '_y',
    ]

    _fields_and_field_types = {
        'x': 'float',
        'y': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.x = kwargs.get('x', float())
        self.y = kwargs.get('y', float())

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


# Import statements for member types

# already imported above
# import rosidl_parser.definition


class Metaclass_ForceBoundaryCheck_Response(type):
    """Metaclass of message 'ForceBoundaryCheck_Response'."""

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
                'service_interface.srv.ForceBoundaryCheck_Response')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__srv__force_boundary_check__response
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__srv__force_boundary_check__response
            cls._CONVERT_TO_PY = module.convert_to_py_msg__srv__force_boundary_check__response
            cls._TYPE_SUPPORT = module.type_support_msg__srv__force_boundary_check__response
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__srv__force_boundary_check__response

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class ForceBoundaryCheck_Response(metaclass=Metaclass_ForceBoundaryCheck_Response):
    """Message class 'ForceBoundaryCheck_Response'."""

    __slots__ = [
        '_is_overwritten',
        '_x',
        '_y',
    ]

    _fields_and_field_types = {
        'is_overwritten': 'boolean',
        'x': 'float',
        'y': 'float',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('boolean'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.is_overwritten = kwargs.get('is_overwritten', bool())
        self.x = kwargs.get('x', float())
        self.y = kwargs.get('y', float())

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
        if self.is_overwritten != other.is_overwritten:
            return False
        if self.x != other.x:
            return False
        if self.y != other.y:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def is_overwritten(self):
        """Message field 'is_overwritten'."""
        return self._is_overwritten

    @is_overwritten.setter
    def is_overwritten(self, value):
        if __debug__:
            assert \
                isinstance(value, bool), \
                "The 'is_overwritten' field must be of type 'bool'"
        self._is_overwritten = value

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


class Metaclass_ForceBoundaryCheck(type):
    """Metaclass of service 'ForceBoundaryCheck'."""

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
                'service_interface.srv.ForceBoundaryCheck')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._TYPE_SUPPORT = module.type_support_srv__srv__force_boundary_check

            from service_interface.srv import _force_boundary_check
            if _force_boundary_check.Metaclass_ForceBoundaryCheck_Request._TYPE_SUPPORT is None:
                _force_boundary_check.Metaclass_ForceBoundaryCheck_Request.__import_type_support__()
            if _force_boundary_check.Metaclass_ForceBoundaryCheck_Response._TYPE_SUPPORT is None:
                _force_boundary_check.Metaclass_ForceBoundaryCheck_Response.__import_type_support__()


class ForceBoundaryCheck(metaclass=Metaclass_ForceBoundaryCheck):
    from service_interface.srv._force_boundary_check import ForceBoundaryCheck_Request as Request
    from service_interface.srv._force_boundary_check import ForceBoundaryCheck_Response as Response

    def __init__(self):
        raise NotImplementedError('Service classes can not be instantiated')

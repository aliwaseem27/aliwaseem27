from math import sqrt, pi, acos
from decimal import Decimal, getcontext

getcontext().prec = 30

class Vector(object):

    CANNOT_NORMALIZE_ZERO_VECTOR_MSG = "Cannot normalize the zero vector"
    NO_UNIQUE_PARALLEL_COMPONENT_MSG = "Cannot find parallel component of zero vector"
    NO_UNIQUE_ORTHOGONAL_COMPONENT_MSG = "Cannot find orthogonal component of zero vector"
    ONLY_DEFINED_IN_TWO_THREE_DIMS_MSG = "Only defined in two or three dimensions systems"

    def __init__(self, coordinates):
        try:
            if not coordinates:
                raise ValueError

            self.coordinates = tuple([Decimal(coor) for coor in coordinates])
            self.dimensions = len(self.coordinates)

        except ValueError:
            print("The coordinates must be nonempty !!")

        except TypeError:
            print("The coordinates must be iterable")

    def __str__(self):
        return "Vector: {}".format(self.coordinates)

    def __eq__(self, other):
        return self.coordinates == other.coordinates

    def __add__(self, other):
        result = [coor1 + coor2 for coor1, coor2 in zip(self.coordinates, other.coordinates)]
        return Vector(result)

    def minus(self, other):
        result = [coor1 - coor2 for coor1, coor2 in zip(self.coordinates, other.coordinates)]
        return Vector(result)

    def multi_by_scalar(self, scalar):
        result = [coor * Decimal(scalar) for coor in self.coordinates]
        return Vector(result)

    def magnitude(self):
        result = sqrt(sum([coor**2 for coor in self.coordinates]))
        return result

    def normalized(self):
        try:
            magnitude = self.magnitude()
            return self.multi_by_scalar(Decimal('1.0') / Decimal(magnitude))

        except ZeroDivisionError:
            raise Exception("Cannot normalize the zero vector !")

    def dot_product(self, other):
        return sum([coor1 * coor2 for coor1, coor2 in zip(self.coordinates, other.coordinates)])

    def angle_with(self, other, in_degrees=False):
        try:
            u1 = self.normalized()
            u2 = other.normalized()
            angle_in_radians = acos(round(u1.dot_product(u2),10))

            if in_degrees:
                degrees_per_radian = 180. / pi
                return angle_in_radians * degrees_per_radian
            else:
                return angle_in_radians
        except Exception as e:
            if str(e) == self.CANNOT_NORMALIZE_ZERO_VECTOR_MSG:
                raise Exception("Cannot compute an angle with the zero vector")
            else:
                raise e

    def is_zero(self, tolerance=1e-10):
        return self.magnitude() < tolerance

    def is_parallel(self, other):
        return (self.is_zero() or other.is_zero() or self.angle_with(other) == 0 or self.angle_with(other) == pi)

    def is_orthogonal(self, other, tolerance=1e-10):
        return abs(self.dot_product(other)) < tolerance

    def component_parallel_to(self, basis):
        try:
            u = basis.normalized()
            weight = self.dot_product(u)
            return u.multi_by_scalar(weight)
        except Exception as e:
            if str(e) == self.CANNOT_NORMALIZE_ZERO_VECTOR_MSG:
                raise Exception(self.NO_UNIQUE_PARALLEL_COMPONENT_MSG)
            else:
                raise e

    def component_orthogonal_to(self, basis):
        try:
            projection = self.component_parallel_to(basis)
            return self.minus(projection)
        except Exception as e:
            if str(e) == self.NO_UNIQUE_PARALLEL_COMPONENT_MSG:
                raise Exception(self.NO_UNIQUE_ORTHOGONAL_COMPONENT_MSG)
            else:
                raise e

    def cross(self,other):
        try:
            x_1, y_1, z_1 = self.coordinates
            x_2, y_2, z_2 = other.coordinates
            new_coordinates = [  y_1 * z_2 - z_1 * y_2,
                               -(x_1 * z_2 - x_2 * z_1),
                                 x_1 * y_2 - x_2 * y_1]
            return Vector(new_coordinates)

        except ValueError as e:
            msg = str(e)
            if msg == "need more than 2 values to unpack":
                self_embedded_in_R3 = Vector(self.coordinates + ('0',))
                other_embedded_in_R3 = Vector(other.coordinates + ('0',))
                return self_embedded_in_R3.cross(other_embedded_in_R3)
            elif (msg == "too many values to unpack" or msg == "need more than 1 value to unpack"):
                raise Exception(self.ONLY_DEFINED_IN_TWO_THREE_DIMS_MSG)
            else:
                raise e

    def area_of_parallelogram_with(self, other):
        cross_product = self.cross(other)
        return cross_product.magnitude()

    def area_of_triangle_with(self, other):
        return self.area_of_parallelogram_with(other) / 2.0



def main():
    vector1 = Vector([8.462,7.893, -8.187])
    vector2 = Vector([6.984, -5.975, 4.778])
    print(vector1.cross(vector2))
    print("----------")

    vector3 = Vector([-8.987, -9.838, 5.031])
    vector4 = Vector([-4.268, -1.861, -8.866])
    # print(vector3.is_parallel(vector4))
    print(vector3.area_of_parallelogram_with(vector4))
    print("----------")
    #
    vector5 = Vector([1.5, 9.547, 3.691])
    vector6 = Vector([-6.007, 0.124, 5.772])
    print(vector5.area_of_triangle_with(vector6))
    # print(vector5.is_orthogonal(vector6))
    print("----------")
    #
    # vector7 = Vector([2.118, 4.827])
    # vector8 = Vector([0, 0])
    # # print(vector7.is_parallel(vector8))
    # print(vector7.is_orthogonal(vector8))
    # print("----------")

    print("ok")
if __name__ == '__main__':
    main()
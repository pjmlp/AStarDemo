export module Node;

import <memory>;
import <iostream>;

/**
 * This class represents nodes in the path
 */
export class Node final
{
public:
	explicit Node(int row, int col) noexcept;

	void set_parent(std::shared_ptr<Node> parent) noexcept { m_parent = parent; }
	std::shared_ptr<Node> get_parent() noexcept { return m_parent; }

	// accessor functions
	int row() const noexcept { return m_row; }
	int col() const noexcept { return m_col; }
	double cost() const noexcept { return m_cost; }
	double estimation() const noexcept { return m_estimation; }
	double total_cost() const noexcept { return m_cost + m_estimation; }

	// setters
	void set_cost(double cost) noexcept { m_cost = cost; }
	void set_estimation(double estimation) noexcept { m_estimation = estimation; }

	bool operator== (const Node& other) const noexcept;
	bool operator!= (const Node& other) const noexcept;

	void write_contents(std::ostream& output) const;

private:
	int m_row, m_col;
	double m_cost;
	double m_estimation;
	std::shared_ptr<Node> m_parent;
};